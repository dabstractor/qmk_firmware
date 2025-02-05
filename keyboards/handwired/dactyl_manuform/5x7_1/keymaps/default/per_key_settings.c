// Keys that should not use "hold on other key press" behavior
const uint16_t non_hold_keys[] = {
    RSFT_T(KC_ENT),  // Right Shift / Enter
    RGUI_T(KC_BSPC)  // Right GUI / Backspace
};

// Keys that should use retro tapping behavior
const uint16_t retro_tapping_keys[] = {
    RSFT_T(KC_ENT),  // Right Shift / Enter - useful for when you want to type uppercase letters
    RGUI_T(KC_BSPC)  // Right GUI / Backspace - allows for easier backspace access
    // RCTL_T(KC_SPC) is excluded as it's typically better with immediate space output
};

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
