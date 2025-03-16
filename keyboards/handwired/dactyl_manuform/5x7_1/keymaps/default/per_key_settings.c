#include "./layers.c"

// Keys that should not use "hold on other key press" behavior (automatically considered a hold if another key is pressed before mod release)
const uint16_t non_hold_keys[] = {
    RSFT_T(KC_ENT),  // Right Shift / Enter
    RGUI_T(KC_BSPC)  // Right GUI / Backspace
};

// Keys that should use retro tapping behavior (if key is held longer than TAPPING_TERM, register the key as a mod hold)
const uint16_t retro_tapping_keys[] = { };

// Array sizes for iteration
#define NON_HOLD_KEYS_SIZE (sizeof(non_hold_keys) / sizeof(uint16_t))
#define RETRO_TAPPING_KEYS_SIZE (sizeof(retro_tapping_keys) / sizeof(uint16_t))

// Check if a key should ignore mod tap interrupt
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < NON_HOLD_KEYS_SIZE; i++) {
        if (keycode == non_hold_keys[i]) {
            return true;
        }
    }
    return false;
}

// Check if a key should use retro tapping
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < RETRO_TAPPING_KEYS_SIZE; i++) {
        if (keycode == retro_tapping_keys[i]) {
            return true;
        }
    }
    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RCTL_T(KC_SPC):
        case RGUI_T(KC_BSPC):
            return TAPPING_TERM + 100;
        case RSFT_T(KC_ENT):
            return TAPPING_TERM - 50;
        case TT(_NUMPAD):
        case TT(_FN):
            return TAPPING_TERM - 100;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(1, KC_BSPC):
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}
