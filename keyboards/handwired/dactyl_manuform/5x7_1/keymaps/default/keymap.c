#include QMK_KEYBOARD_H
#include "qmk-vim/src/vim.h"
#include "leader.h"
#include "./secrets.h"
#include "./keycombos.h"
#include "./custom_mod_map.c"
#include "./per_key_settings.c"
#include "./custom_keycodes.c"
#include "./layers.c"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x7_1(
        // left hand
        KC_GRV,           KC_1,        KC_2,    KC_3,   KC_4,   KC_5,   TG(_CMD),
        KC_TAB,           KC_Q,        KC_W,    KC_E,   KC_R,   KC_T,   TG(_NUMPAD),
        KC_ESC,           KC_A,        KC_S,    KC_D,   KC_F,   KC_G,   KC_PGUP,
        LSFT_T(KC_CAPS),  KC_Z,        KC_X,    KC_C,   KC_V,   KC_B,   KC_PGDN,
        TOGGLE_VIM,       QK_LEAD,     KC_LBRC, KC_RBRC,
                                                      KC_LSFT,     KC_LCTL,
                                                      KC_LALT,     KC_LGUI,
                                                      TT(_NUMPAD), TT(_FN),
                                                      KC_MS_BTN4,
        // right hand
        KC_PRINT_SCREEN,  KC_6,  KC_7,   KC_8,     KC_9,    KC_0,     KC_DEL,
        KC_MS_BTN1,       KC_Y,  KC_U,   KC_I,     KC_O,    KC_P,     KC_BSLS,
        KC_HOME,          KC_H,  KC_J,   KC_K,     KC_L,    KC_SCLN,  KC_QUOT,
        KC_END,           KC_N,  KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,  KC_RSFT,
                                         KC_MINS,  KC_EQL,  KC_LBRC,  KC_RBRC,
        RCTL_T(KC_SPC),   RSFT_T(KC_ENT),
        RGUI_T(KC_BSPC),  RALT_T(KC_BSPC),
        QK_LEAD,          KC_LALT,
        KC_MS_BTN5
    ),

    [_FN] = LAYOUT_5x7_1(
        // left hand
        QK_BOOT,     KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,    _______,
        _______,     KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,    _______,
        TO(_QWERTY), _______,   _______,   KC_UP,     _______,   _______,  _______,
        _______,     _______,   KC_LEFT,   KC_DOWN,   KC_RGHT,   _______,  _______,
        KC_MSTP,     KC_MPLY,   KC_MPRV,   KC_MNXT,
                                                                 _______, _______,
                                                                 _______, _______,
                                                                 _______, _______,
                                                                 KC_Z,
        // right hand
        KC_F11,   KC_F6,       KC_F7,      KC_F8,    KC_F9,       KC_F10,    QK_BOOT,
        KC_F12,   KC_F6,       KC_F7,      KC_F8,    KC_F9,       KC_F10,    _______,
        KC_F13,   KC_MS_BTN4,  _______,    KC_UP,    KC_MS_BTN5,  _______,   _______,
        KC_F14,   _______,     KC_LEFT,    KC_DOWN,  KC_RGHT,     _______,   _______,
                                           _______,   _______,    _______,   TOGGLE_VIM,
        _______,  _______,
        KC_DEL,   _______,
        _______,  _______,
        KC_Y
    ),

    [_NUMPAD] = LAYOUT_5x7_1(
        // left hand
        _______,      _______,  _______,  _______,  _______,  _______,  KC_CALC,
        _______,      KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     _______,
        TO(_QWERTY),  _______,  _______,  _______,  _______,  _______,  _______,
        _______,      _______,  _______,  _______,  _______,  _______,  _______,
        _______,      _______,  _______,  _______,
                                                              _______,  _______,
                                                              _______,  _______,
                                                              _______,  _______,
                                                              KC_Z,
        // right hand
        KC_NUM,   _______,  _______,  _______,  KC_PMNS,  KC_PPLS,  _______,
        KC_CALC,  KC_6,     KC_P7,    KC_P8,    KC_P9,    KC_0,     _______,
        _______,  _______,  KC_P4,    KC_P5,    KC_P6,    KC_PAST,  _______,
        _______,  _______,  KC_P1,    KC_P2,    KC_P3,    KC_PSLS,  _______,
                                      KC_P0,    KC_PDOT,  KC_PENT,  _______,
        _______,  KC_PENT,
        _______,  _______,
        _______,  _______,
        KC_Y
    ),

    [_CMD] = LAYOUT_5x7_1(
        // left hand
        _______,      _______,   _______,   _______,   _______,   _______,  QK_DYNAMIC_MACRO_RECORD_START_1,
        _______,      _______,   _______,   _______,   _______,   _______,  QK_DYNAMIC_MACRO_RECORD_STOP,
        TO(_QWERTY),  _______,   _______,   _______,   _______,   _______,  QK_DYNAMIC_MACRO_PLAY_1,
        _______,      _______,   _______,   _______,   _______,   _______,  _______,
        _______,      _______,   _______,   _______,
                                                                  _______, _______,
                                                                  _______, _______,
                                                                  _______, _______,
                                                                  KC_Z,
        // right hand
        QK_DYNAMIC_MACRO_RECORD_START_2,  _______,     _______,   _______,   _______,     _______,   _______,
        QK_DYNAMIC_MACRO_RECORD_STOP,     _______,     _______,   _______,   _______,     _______,   _______,
        QK_DYNAMIC_MACRO_PLAY_2,          KC_MS_BTN4,  _______,   _______,   KC_MS_BTN5,  _______,   _______,
        _______,                          _______,     _______,   _______,   _______,     _______,   _______,
                                          _______,     _______,   _______,   _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        KC_Y
    ),

    [_MOUSE] = LAYOUT_5x7_1(
        // left hand
        _______,      _______,  _______,     _______,     _______,      _______,  _______,
        _______,      _______,  KC_MS_BTN3,  KC_MS_BTN2,  KC_MS_BTN1,   _______,  _______,
        TO(_QWERTY),  _______,  _______,     KC_MS_UP,    _______,      _______,  _______,
        _______,      _______,  KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT,  _______,  _______,
        _______,      _______,  _______,     _______,
                                                                        _______,  _______,
                                                                        _______,  _______,
                                                                        _______,  _______,
                                                                        KC_Z,
        _______,  _______,     _______,     _______,     _______,      _______,  _______,
        _______,  _______,     KC_MS_BTN1,  KC_MS_BTN2,  KC_MS_BTN3,   _______,  _______,
        _______,  KC_MS_BTN4,  _______,     KC_MS_UP,    KC_MS_BTN5,   _______,  _______,
        _______,  _______,     KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT,  _______,  _______,
                                            _______,     _______,      _______,  _______,
        _______,  _______,
        _______,    _______,
        TT(_FN),    TT(_NUMPAD),
        KC_Y
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
                  // left                                        // right
    [_QWERTY] = { ENCODER_CCW_CW(KC_MS_WH_LEFT, KC_MS_WH_RIGHT), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    /* [_QWERTY] = { ENCODER_CCW_CW(WORKSPACE_LEFT, WORKSPACE_RIGHT), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  }, */
    [_FN]     = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),                ENCODER_CCW_CW(TAB_L, TAB_R) },
    [_NUMPAD] = { ENCODER_CCW_CW(KC_UP, KC_DOWN),                  ENCODER_CCW_CW(KC_LEFT, KC_RIGHT) },
    [_CMD]    = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),               ENCODER_CCW_CW(KC_BSPC, KC_SPC) },
    [_MOUSE]  = { ENCODER_CCW_CW(KC_MS_UP, KC_MS_DOWN),            ENCODER_CCW_CW(KC_MS_LEFT, KC_MS_RIGHT) },
};
/* ENCODER_CCW_CW(KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP) */
#endif

#if defined(LEADER_ENABLE)
// accepts up to 5 keys
KeyCombo leader_keys[] = {
    { KEYS(KC_U, KC_A), USERNAME1 },
    { KEYS(KC_U, KC_S), USERNAME2 },
    { KEYS(KC_U, KC_D), USERNAME3 },
    { KEYS(KC_P, KC_A), PASSWORD1 SS_TAP(X_ENTER) },
    { KEYS(KC_P, KC_S), PASSWORD2 SS_TAP(X_ENTER) },
    { KEYS(KC_P, KC_D), PASSWORD2 SS_TAP(X_ENTER) },
    { KEYS(KC_R, KC_I), "/right\n" },
    { KEYS(KC_L, KC_E), "/left\n" },
    { KEYS(KC_J, KC_I), SS_TAP(X_HOME) "https://jitsi.mulletware.io/\n" }  ,
    { KEYS(KC_1), "!-1:" },
    { KEYS(KC_G, KC_P), SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_ENTER) }

};

void leader_end_user(void) {
    for (size_t i = 0; i < sizeof(leader_keys) / sizeof(leader_keys[0]); ++i) {
        if (leader_keys[i].count > 0 && leader_keys[i].count <= MAX_COMBO_KEYS) {
            if (leader(leader_keys[i].keys, leader_keys[i].count)) {
                SEND_STRING(leader_keys[i].output);
            }
        }
    }
}
#endif // LEADER

void qmk_leader(uint16_t keycode, keyrecord_t *record) {
    leader_sequence_active() ? leader_end() : leader_start();
}

void term_toggle(uint16_t keycode, keyrecord_t *record) {
    // send GUI+Space
    register_code(KC_RALT);
    register_code(KC_SPC);
    unregister_code(KC_RALT);
    unregister_code(KC_SPC);
}

bool is_dbl_spc_window_active = false;

void dbl_spc_toggle(uint16_t keycode, keyrecord_t *record) {
    // toggles neovim open current buffer window
    if (is_dbl_spc_window_active) {
        // send esc key
        register_code(KC_ESC);
        unregister_code(KC_ESC);
        register_code(KC_ESC);
        unregister_code(KC_ESC);
        is_dbl_spc_window_active = false;
    } else {
        send_string("  ");
        is_dbl_spc_window_active = true;
    }
}

void tmux_leader(uint16_t keycode, keyrecord_t *record) {
    // send Ctrl+Space
    register_code(KC_RCTL);
    register_code(KC_SPC);
    unregister_code(KC_RCTL);
    unregister_code(KC_SPC);
}

void activate_caps_word_toggle(uint16_t keycode, keyrecord_t *record) {
    caps_word_toggle();
}

void toggle_mouse_layer(uint16_t keycode, keyrecord_t *record) {
    layer_invert(_MOUSE);
}

KeyCallback CUSTOM_MOD_TAP_KEYS[] = {
    { KC_LGUI, term_toggle },
    { KC_LCTL, tmux_leader },
    { KC_LSFT, activate_caps_word_toggle },
    { KC_RSFT, toggle_mouse_layer },
};


bool process_custom_mod_tap_user(uint16_t keycode, keyrecord_t *record) {
    bool found_key = false;

    for (size_t i = 0; i < sizeof(CUSTOM_MOD_TAP_KEYS) / sizeof(CUSTOM_MOD_TAP_KEYS[0]); ++i) {
        if (CUSTOM_MOD_TAP_KEYS[i].keycode == keycode) {
            found_key = true;

            if (record->event.pressed) {
                on_press(keycode);
                if (CUSTOM_MOD_TAP_KEYS[i].on_press != NULL) {
                    CUSTOM_MOD_TAP_KEYS[i].on_press(keycode, record);
                }
                return false;
            } else {
                if (on_release(keycode) &&
                    CUSTOM_MOD_TAP_KEYS[i].on_release != NULL) {
                    CUSTOM_MOD_TAP_KEYS[i].on_release(keycode, record);
                }
                return false;
            }

            return true;
        }
    }

    if (!found_key && record->event.pressed) on_other_press();

    return true;
}

void double_space(uint16_t keycode, keyrecord_t *record) {
    send_string("  ");
}

void toggle_vim(uint16_t keycode, keyrecord_t *record) {
    toggle_vim_mode();
}

void click_this_spot(uint16_t keycode, keyrecord_t *record) {
    tap_code(KC_MS_UP);
    tap_code(KC_MS_DOWN);
    tap_code(KC_MS_BTN1);
}

void workspace_left(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_RGUI);
    register_code(KC_H);
    unregister_code(KC_H);
    unregister_code(KC_RGUI);
}

void workspace_right(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_RGUI);
    register_code(KC_L);
    unregister_code(KC_L);
    unregister_code(KC_RGUI);
}

KeyCallback CUSTOM_KEYS[] = {
    { DBL_SPACE, double_space },
    { TOGGLE_VIM, toggle_vim },
    { CLICK_THIS_SPOT, click_this_spot },
    { WORKSPACE_LEFT, workspace_left },
    { WORKSPACE_RIGHT, workspace_right },
};

bool process_custom_key_user(uint16_t keycode, keyrecord_t *record) {
    for (size_t i = 0; i < sizeof(CUSTOM_KEYS) / sizeof(CUSTOM_KEYS[0]); ++i) {
        if (CUSTOM_KEYS[i].keycode == keycode) {
            if (record->event.pressed) {
                if (CUSTOM_KEYS[i].on_press != NULL) {
                    CUSTOM_KEYS[i].on_press(keycode, record);
                }
            } else {
                if (CUSTOM_KEYS[i].on_release != NULL) {
                    CUSTOM_KEYS[i].on_release(keycode, record);
                }
            }
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

#ifdef CONSOLE_ENABLE // Logging
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    if (!process_vim_mode(keycode, record)) return false;

    process_custom_key_user(keycode, record);
    return process_custom_mod_tap_user(keycode, record);
}

// mac mode for vim
bool process_detected_host_os_kb(os_variant_t detected_os) {
    if (!process_detected_host_os_user(detected_os)) {
        return false;
    }

    if (detected_os == OS_MACOS) {
        enable_vim_for_mac();
    }

    return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // when layers _FN and _NUMPAD are active, _CMD is active
    state = update_tri_layer_state(state, _FN, _NUMPAD, _CMD);
    return state;
}
