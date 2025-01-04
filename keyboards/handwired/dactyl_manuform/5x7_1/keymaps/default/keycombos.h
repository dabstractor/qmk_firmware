#include QMK_KEYBOARD_H

typedef struct {
    char keys[2];
    const char* string;
} TwoKeyCombo;

TwoKeyCombo two_key_combos[] = {
    { { KC_U, KC_A }, USERNAME1 },
    { { KC_U, KC_S }, USERNAME2 },
    { { KC_U, KC_D }, USERNAME3 },
    { { KC_P, KC_A }, PASSWORD1 },
    { { KC_P, KC_S }, PASSWORD2 },
    { { KC_P, KC_D }, PASSWORD2 },
    { { KC_J, KC_I }, "https://jitsi.mulletware.io/" },
    { { KC_R, KC_I }, "/right" }
};

#define TWO_KEY_COMBOS_COUNT (sizeof(two_key_combos) / sizeof(two_key_combos[0]))
