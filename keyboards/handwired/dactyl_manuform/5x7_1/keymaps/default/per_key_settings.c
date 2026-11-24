#include "./layers.c"

// Per-key hold tables live in keymap.c (the single edit surface); extern
// declarations here let the QMK callbacks below iterate them.
extern const uint16_t hold_keys[];
extern const uint8_t hold_keys_size;
extern const uint16_t retro_tapping_keys[];
extern const uint8_t retro_tapping_keys_size;

// Check if a key should enable mod hold on key down instead of waiting for it to release first
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < hold_keys_size; i++) {
        if (keycode == hold_keys[i]) {
            return true;
        }
    }
    return false;
}

// Check if a key should use retro tapping
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < retro_tapping_keys_size; i++) {
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
