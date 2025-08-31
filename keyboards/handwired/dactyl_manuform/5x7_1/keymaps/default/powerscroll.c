#include QMK_KEYBOARD_H
#include "./custom_keycodes.h"
#include "./qmk-vim/src/vim.h"
#include "./powerscroll.h"
#include "keymap_introspection.h"

#ifndef POWERSCROLL_LEVEL
#    define POWERSCROLL_LEVEL 10
#endif

#ifndef POWERSCROLL_DELAY_MS
#    define POWERSCROLL_DELAY_MS 10
#endif

#ifndef MAX_POWERSCROLL_STREAMS
#    define MAX_POWERSCROLL_STREAMS 4
#endif

// Activator: custom keycode POWERSCROLL_HOLD. While held, multiply encoder output.

// Forward declarations for existing helpers defined elsewhere
bool process_custom_mod_tap_user(uint16_t keycode, keyrecord_t *record);
bool process_custom_key_user(uint16_t keycode, keyrecord_t *record);
void check_vim_lazy_insert(bool pressed);

// Multi-stream powerscroll state structure
typedef struct {
    bool is_active;              // Stream currently running
    uint16_t last_tap_time;      // Last time we sent a scroll event (timer_read() format)
    uint8_t remaining_taps;      // How many more scroll events to send
    uint16_t keycode;            // Which keycode to send (KC_MS_WH_UP, etc.)
    uint8_t tap_interval_ms;     // Delay between taps (POWERSCROLL_DELAY_MS)
    uint8_t stream_id;           // Unique identifier for debugging
} PowerscrollStream;

// Global streams array supporting parallel powerscroll operations
static PowerscrollStream powerscroll_streams[MAX_POWERSCROLL_STREAMS];
static uint8_t next_stream_id = 1;

// Count of powerscroll holds currently pressed; stacking allowed
static uint8_t s_powerscroll_hold_count = 0;

// Auto-populated conflict storage (max 20 conflict pairs should be plenty)
#define MAX_AUTO_CONFLICTS 20
PowerscrollConflictMap POWERSCROLL_AUTO_CONFLICTS[MAX_AUTO_CONFLICTS];
size_t POWERSCROLL_AUTO_CONFLICTS_COUNT = 0;

// Helper function to add conflicts during runtime
void powerscroll_add_conflict(uint16_t ccw, uint16_t cw) {
    // Skip if we've reached max conflicts or if either keycode is transparent
    if (POWERSCROLL_AUTO_CONFLICTS_COUNT >= MAX_AUTO_CONFLICTS || ccw == KC_TRNS || cw == KC_TRNS) {
        return;
    }

    // Check if this conflict pair already exists (avoid duplicates)
    for (size_t i = 0; i < POWERSCROLL_AUTO_CONFLICTS_COUNT; i++) {
        if ((POWERSCROLL_AUTO_CONFLICTS[i].keycode == ccw && POWERSCROLL_AUTO_CONFLICTS[i].conflicts_with == cw) ||
            (POWERSCROLL_AUTO_CONFLICTS[i].keycode == cw && POWERSCROLL_AUTO_CONFLICTS[i].conflicts_with == ccw)) {
            return; // Already exists
        }
    }

    // Add the conflict pair
    POWERSCROLL_AUTO_CONFLICTS[POWERSCROLL_AUTO_CONFLICTS_COUNT].keycode = ccw;
    POWERSCROLL_AUTO_CONFLICTS[POWERSCROLL_AUTO_CONFLICTS_COUNT].conflicts_with = cw;
    POWERSCROLL_AUTO_CONFLICTS_COUNT++;
}

// Initialize conflicts from encoder map at startup
void powerscroll_init_conflicts(void) {
    // Clear any existing conflicts
    POWERSCROLL_AUTO_CONFLICTS_COUNT = 0;

    // Iterate through all layers and encoder definitions
    for (uint8_t layer = 0; layer < encodermap_layer_count(); layer++) {
        for (uint8_t encoder = 0; encoder < NUM_ENCODERS; encoder++) {
            uint16_t ccw = encoder_map[layer][encoder][0];  // Counter-clockwise
            uint16_t cw = encoder_map[layer][encoder][1];   // Clockwise

            // Add conflict pair if both keycodes are valid (not transparent)
            if (ccw != KC_TRNS && cw != KC_TRNS) {
                powerscroll_add_conflict(ccw, cw);
            }
        }
    }

    uprintf("POWERSCROLL: Auto-initialized %d conflict pairs from encoder map\n", (int)POWERSCROLL_AUTO_CONFLICTS_COUNT);
}

// Translate encoder keycode back to original keycode for conflict lookup
// QMK's encoder system applies a -12 offset to certain keycodes
static uint16_t encoder_to_original_keycode(uint16_t encoder_keycode) {
    // Mouse wheel range (0x00CF-0x00D2 maps to 0x00DB-0x00DE)
    if (encoder_keycode >= 0x00CF && encoder_keycode <= 0x00D2) {
        return encoder_keycode + 12;
    }
    // Check if it's in a basic keycode range that might also have the offset
    // We'll detect this dynamically by testing different ranges
    if (encoder_keycode >= 0x0014 && encoder_keycode <= 0x0042) {
        // This might be the basic keycode range with offset applied
        return encoder_keycode + 12;
    }
    return encoder_keycode;
}

// Translate original keycode to encoder keycode for return value
static uint16_t original_to_encoder_keycode(uint16_t original_keycode) {
    // Mouse wheel range (0x00DB-0x00DE maps to 0x00CF-0x00D2)
    if (original_keycode >= 0x00DB && original_keycode <= 0x00DE) {
        return original_keycode - 12;
    }
    // Basic keycode range (like KC_SPC=0x2C, KC_BSPC=0x2A might map to 0x20, 0x1E)
    if (original_keycode >= 0x0020 && original_keycode <= 0x004E) {
        return original_keycode - 12;
    }
    return original_keycode;
}

// Helper function for conflict resolution - implementation kept in powerscroll.c
static uint16_t get_conflicting_keycode(uint16_t keycode) {
    uprintf("POWERSCROLL: get_conflicting_keycode() called with encoder keycode 0x%04X\n", keycode);
    uprintf("POWERSCROLL: auto-conflicts count: %d\n", (int)POWERSCROLL_AUTO_CONFLICTS_COUNT);

    // Convert encoder keycode to original for lookup
    uint16_t original_keycode = encoder_to_original_keycode(keycode);
    uprintf("POWERSCROLL: translated to original keycode 0x%04X\n", original_keycode);

    for (size_t i = 0; i < POWERSCROLL_AUTO_CONFLICTS_COUNT; ++i) {
        uprintf("POWERSCROLL: checking auto-rule %d: 0x%04X vs 0x%04X\n",
               (int)i, POWERSCROLL_AUTO_CONFLICTS[i].keycode, POWERSCROLL_AUTO_CONFLICTS[i].conflicts_with);

        if (POWERSCROLL_AUTO_CONFLICTS[i].keycode == original_keycode) {
            uint16_t conflicts_with = POWERSCROLL_AUTO_CONFLICTS[i].conflicts_with;
            uint16_t encoder_conflicts_with = original_to_encoder_keycode(conflicts_with);
            uprintf("POWERSCROLL: found match on keycode, returning encoder conflicts_with=0x%04X (original=0x%04X)\n",
                   encoder_conflicts_with, conflicts_with);
            return encoder_conflicts_with;
        } else if (POWERSCROLL_AUTO_CONFLICTS[i].conflicts_with == original_keycode) {
            uint16_t conflicts_with = POWERSCROLL_AUTO_CONFLICTS[i].keycode;
            uint16_t encoder_conflicts_with = original_to_encoder_keycode(conflicts_with);
            uprintf("POWERSCROLL: found match on conflicts_with, returning encoder keycode=0x%04X (original=0x%04X)\n",
                   encoder_conflicts_with, conflicts_with);
            return encoder_conflicts_with;
        }
    }
    uprintf("POWERSCROLL: no auto-conflicts found, returning KC_NO\n");
    return KC_NO;
}

// Simple conflict resolution - much cleaner like CustomModMap
static void stop_conflicting_streams(uint16_t new_keycode) {
    uprintf("POWERSCROLL: checking conflicts for keycode 0x%04X\n", new_keycode);

    // Use helper function (keeps keymap.c clean - only config)
    uint16_t conflicts_with = get_conflicting_keycode(new_keycode);

    // If no conflict mapping found, nothing to stop
    if (conflicts_with == KC_NO) {
        uprintf("POWERSCROLL: no conflicts found for keycode 0x%04X\n", new_keycode);
        return;
    }

    uprintf("POWERSCROLL: keycode 0x%04X conflicts with 0x%04X\n", new_keycode, conflicts_with);

    // Stop any active stream running the conflicting keycode
    int stopped_streams = 0;
    for (int i = 0; i < MAX_POWERSCROLL_STREAMS; i++) {
        if (powerscroll_streams[i].is_active && powerscroll_streams[i].keycode == conflicts_with) {
            uprintf("POWERSCROLL: STOPPING conflicting stream %d (keycode 0x%04X)\n", i, conflicts_with);
            powerscroll_streams[i].is_active = false;
            stopped_streams++;
        }
    }

    uprintf("POWERSCROLL: stopped %d conflicting streams\n", stopped_streams);
}

// Core non-blocking scheduling function supporting parallel streams
static void powerscroll_schedule_taps(uint16_t keycode, uint8_t times) {
    if (times == 0) return;

    uprintf("POWERSCROLL: schedule_taps() called with keycode 0x%04X, times=%d\n", keycode, times);

    // Stop any conflicting streams before starting this one
    stop_conflicting_streams(keycode);

    int8_t stream_slot = -1;

    // First, look for existing stream with same keycode to extend it
    for (int i = 0; i < MAX_POWERSCROLL_STREAMS; i++) {
        if (powerscroll_streams[i].is_active && powerscroll_streams[i].keycode == keycode) {
            stream_slot = i;
            break;
        }
    }

    // If no existing stream, find available slot
    if (stream_slot == -1) {
        for (int i = 0; i < MAX_POWERSCROLL_STREAMS; i++) {
            if (!powerscroll_streams[i].is_active) {
                stream_slot = i;
                break;
            }
        }
    }

    // If no available slots, use oldest active stream
    if (stream_slot == -1) {
        stream_slot = 0;
        uint16_t oldest_time = powerscroll_streams[0].last_tap_time;
        for (int i = 1; i < MAX_POWERSCROLL_STREAMS; i++) {
            if (timer_elapsed(powerscroll_streams[i].last_tap_time) > timer_elapsed(oldest_time)) {
                stream_slot = i;
                oldest_time = powerscroll_streams[i].last_tap_time;
            }
        }
    }

    // Initialize or extend the stream
    PowerscrollStream* stream = &powerscroll_streams[stream_slot];
    if (stream->is_active && stream->keycode == keycode) {
        // Extend existing stream by adding more taps
        uprintf("POWERSCROLL: extending existing stream %d (ID %d) by %d taps (was %d, now %d)\n",
               stream_slot, stream->stream_id, times, stream->remaining_taps, stream->remaining_taps + times);
        stream->remaining_taps += times;
    } else {
        // Initialize new stream
        stream->is_active = true;
        stream->remaining_taps = times;
        stream->keycode = keycode;
        stream->tap_interval_ms = POWERSCROLL_DELAY_MS;
        stream->last_tap_time = timer_read();
        stream->stream_id = next_stream_id++;
        if (next_stream_id == 0) next_stream_id = 1; // Avoid ID 0

        uprintf("POWERSCROLL: created NEW stream %d (ID %d) for keycode 0x%04X with %d taps\n",
               stream_slot, stream->stream_id, keycode, times);

        // Send first tap immediately for responsiveness
        tap_code16(keycode);
        stream->remaining_taps--;
        uprintf("POWERSCROLL: sent immediate tap, %d remaining\n", stream->remaining_taps);
    }
}

// Matrix scan processing function supporting parallel streams
void powerscroll_matrix_scan(void) {
    // Process all active streams in parallel
    for (int i = 0; i < MAX_POWERSCROLL_STREAMS; i++) {
        PowerscrollStream* stream = &powerscroll_streams[i];

        if (!stream->is_active) continue;

        // Check if enough time has elapsed for next tap in this stream
        if (timer_elapsed(stream->last_tap_time) >= stream->tap_interval_ms) {
            if (stream->remaining_taps > 0) {
                // Send next scroll event for this stream
                tap_code16(stream->keycode);
                stream->remaining_taps--;
                stream->last_tap_time = timer_read();  // Reset timer
            } else {
                // Stream sequence complete
                stream->is_active = false;
            }
        }
    }
}

// Returns true to allow normal processing, false to consume
// Focused only on powerscroll behavior to integrate with existing hook logic
bool powerscroll_process(uint16_t keycode, keyrecord_t *record) {
    // Track the dedicated powerscroll hold key (stacking)
    if (keycode == POWERSCROLL_HOLD) {
        if (record->event.pressed) {
            if (s_powerscroll_hold_count < UINT8_MAX) s_powerscroll_hold_count++;
            uprintf("POWERSCROLL: POWERSCROLL_HOLD pressed, count now %d\n", s_powerscroll_hold_count);
        } else {
            if (s_powerscroll_hold_count > 0) s_powerscroll_hold_count--;
            uprintf("POWERSCROLL: POWERSCROLL_HOLD released, count now %d\n", s_powerscroll_hold_count);
        }
        return true;
    }
#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
    // When using encoder_map, encoder turns arrive as ENCODER events carrying the mapped keycode
    if (IS_ENCODEREVENT(record->event)) {
        uprintf("POWERSCROLL: ENCODER event detected, keycode=0x%04X, pressed=%d\n", keycode, record->event.pressed);

        if (s_powerscroll_hold_count == 0) {
            uprintf("POWERSCROLL: powerscroll not active, passthrough\n");
            return true; // not powerscrolling, passthrough
        }

        if (record->event.pressed) {
            // repeats = POWERSCROLL_LEVEL ^ s_powerscroll_hold_count, saturated and bounded to 255
            uint16_t repeats = 1;
            for (uint8_t i = 0; i < s_powerscroll_hold_count; i++) {
                if (repeats > (UINT16_MAX / POWERSCROLL_LEVEL)) { repeats = UINT16_MAX; break; }
                repeats = (uint16_t)(repeats * POWERSCROLL_LEVEL);
            }
            if (repeats > 255) repeats = 255;
            uprintf("POWERSCROLL: encoder powerscroll activated, keycode=0x%04X, hold_count=%d, repeats=%d\n",
                   keycode, s_powerscroll_hold_count, repeats);
            powerscroll_schedule_taps(keycode, (uint8_t)repeats);
            return false; // Consume original press
        } else {
            uprintf("POWERSCROLL: consuming encoder release for keycode 0x%04X\n", keycode);
            return false; // Consume the paired release
        }
    }
#endif

    return true;
}


// Encoder hook for non-ENCODER_MAP configurations; safe no-op passthrough otherwise
bool handle_encoder_update(uint8_t index, bool clockwise) {
    // If encoder_map is enabled, this hook won't be called; keep passthrough semantics
    (void)index;
    (void)clockwise;
    return true;
}
