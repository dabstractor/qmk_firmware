#include QMK_KEYBOARD_H
#include "print.h"

#define KEYS(...) { __VA_ARGS__, KC_NO }

#define MAX_COMBO_KEYS 5

typedef struct {
    char keys[MAX_COMBO_KEYS + 1];
    const char* output;
} KeyCombo;

// accepts up to 5 keys
KeyCombo key_combos[] = {
    { KEYS(KC_U, KC_A), USERNAME1 },
    { KEYS(KC_U, KC_S), USERNAME2 },
    { KEYS(KC_U, KC_D), USERNAME3 },
    { KEYS(KC_P, KC_A), PASSWORD1 SS_TAP(X_ENTER) },
    { KEYS(KC_P, KC_S), PASSWORD2 SS_TAP(X_ENTER) },
    { KEYS(KC_P, KC_D), PASSWORD2 SS_TAP(X_ENTER) },
    { KEYS(KC_R, KC_I), "/right\n" },
    { KEYS(KC_L, KC_E), "/left\n" },
    { KEYS(KC_J, KC_I), SS_TAP(X_HOME) "https://jitsi.mulletware.io/\n" }
};

#define KEY_COMBOS_COUNT (sizeof(key_combos) / sizeof(key_combos[0]))

