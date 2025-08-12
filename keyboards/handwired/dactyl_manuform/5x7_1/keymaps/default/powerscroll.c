#include QMK_KEYBOARD_H
#include "./custom_keycodes.h"
#include "./qmk-vim/src/vim.h"

// Centralized, single-file rotary encoder powerscroll logic
// Requirements:
// - One function call per QMK hook
// - No additional layers
// - Preserve existing keymap and encoder_map

#ifndef POWERSCROLL_FACTOR
#    define POWERSCROLL_FACTOR 10
#endif

#ifndef POWERSCROLL_DELAY_MS
#    define POWERSCROLL_DELAY_MS 2
#endif

// Activator: custom keycode POWERSCROLL_HOLD. While held, multiply encoder output.

// Forward declarations for existing helpers defined elsewhere
bool process_custom_mod_tap_user(uint16_t keycode, keyrecord_t *record);
bool process_custom_key_user(uint16_t keycode, keyrecord_t *record);
void check_vim_lazy_insert(bool pressed);

// Count of powerscroll holds currently pressed; stacking allowed
static uint8_t s_powerscroll_hold_count = 0;

static inline void tap_code16_n_times(uint16_t keycode, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        tap_code16(keycode);
        wait_ms(POWERSCROLL_DELAY_MS); // delay between discrete taps
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
            // repeats = POWERSCROLL_FACTOR ^ s_powerscroll_hold_count, saturated and bounded to 255
            uint16_t repeats = 1;
            for (uint8_t i = 0; i < s_powerscroll_hold_count; i++) {
                if (repeats > (UINT16_MAX / POWERSCROLL_FACTOR)) { repeats = UINT16_MAX; break; }
                repeats = (uint16_t)(repeats * POWERSCROLL_FACTOR);
            }
            if (repeats > 255) repeats = 255;
            tap_code16_n_times(keycode, (uint8_t)repeats);
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
