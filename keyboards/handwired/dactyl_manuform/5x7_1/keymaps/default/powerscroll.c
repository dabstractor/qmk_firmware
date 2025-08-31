#include QMK_KEYBOARD_H
#include "./custom_keycodes.h"
#include "./qmk-vim/src/vim.h"

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

// Core non-blocking scheduling function supporting parallel streams
static void powerscroll_schedule_taps(uint16_t keycode, uint8_t times) {
    if (times == 0) return;
    
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
        
        // Send first tap immediately for responsiveness
        tap_code16(keycode);
        stream->remaining_taps--;
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
        } else {
            if (s_powerscroll_hold_count > 0) s_powerscroll_hold_count--;
        }
        return true;
    }
#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
    // When using encoder_map, encoder turns arrive as ENCODER events carrying the mapped keycode
    if (IS_ENCODEREVENT(record->event)) {
        if (s_powerscroll_hold_count == 0) return true; // not powerscrolling, passthrough

        if (record->event.pressed) {
            // repeats = POWERSCROLL_LEVEL ^ s_powerscroll_hold_count, saturated and bounded to 255
            uint16_t repeats = 1;
            for (uint8_t i = 0; i < s_powerscroll_hold_count; i++) {
                if (repeats > (UINT16_MAX / POWERSCROLL_LEVEL)) { repeats = UINT16_MAX; break; }
                repeats = (uint16_t)(repeats * POWERSCROLL_LEVEL);
            }
            if (repeats > 255) repeats = 255;
            powerscroll_schedule_taps(keycode, (uint8_t)repeats);
            return false; // Consume original press
        } else {
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
