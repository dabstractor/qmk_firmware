#include QMK_KEYBOARD_H
#include "./qmk-vim/src/vim.h"
#include "./qmk-notifier/notifier.h"
#include "./secrets.h"
#include "./keycombos.h"
#include "./custom_mod_map.c"
#include "./per_key_settings.c"
#include "./custom_keycodes.c"
#include "./layers.c"
#include "./macros.c"
#include "./tap_dance.c"
#include "./serial_command.c"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x7_1(
        // left hand
        KC_GRV,           KC_1,        KC_2,     KC_3,   KC_4,   KC_5,   KC_HOME,
        KC_TAB,           KC_Q,        KC_W,     KC_E,   KC_R,   KC_T,   KC_END,
        KC_ESC,           KC_A,        KC_S,     KC_D,   KC_F,   KC_G,   KC_PGUP,
        LSFT_T(KC_CAPS),  KC_Z,        KC_X,     KC_C,   KC_V,   KC_B,   KC_PGDN,
        MO(_CMD),         QK_LEAD,     KC_LBRC,  KC_RBRC,
                                                 CAPS_WORD_TOGGLE,  TMUX_LEADER,
                                                 KC_LALT,           TERM_TOGGLE,
                                                 TT(_NUMPAD),       TT(_FN),
                                                 KC_MS_BTN4,

        // right hand
        KC_MS_BTN2,  KC_6,  KC_7,   KC_8,     KC_9,    KC_0,              KC_DEL,
        KC_MS_BTN1,  KC_Y,  KC_U,   KC_I,     KC_O,    KC_P,              KC_BSLS,
        KC_UP,       KC_H,  KC_J,   KC_K,     KC_L,    KC_SCLN,           KC_QUOT,
        KC_DOWN,     KC_N,  KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,           TOGGLE_MOUSE,
                                    KC_MINS,  KC_EQL,  MO(_POWERSCROLL),  TG(_MOUSE),
        RCTL_T(KC_SPC),   RSFT_T(KC_ENT),
        RGUI_T(KC_BSPC),  KC_RALT,
        TD(TD_LEAD_FN),   TT(_NUMPAD),
        KC_MS_BTN5
    ),

    [_NEOVIM] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  KC_F19,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_F20,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_F21,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_F22,
        _______,  _______,  _______,  _______,
                                                          _______,  VIM_BUFFERS,
                                                          COLON,    _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  RESUME_SEARCH,
        _______,  _______,
        _______
    ),

    [_CHROME] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                                          _______,  CHROME_URL_BAR,
                                                          CTRL_K,   _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  TAB_EXTRACT,
        _______,  _______,
        _______
    ),

    [_TERMINAL] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KILL_RUN,
        _______,  _______,  _______,  _______,
                                                          _______,        TMUX_LEADER,
                                                          TMUX_SESSIONX,  _______,
                                                          _______,        _______,
                                                          TMUX_TAB_PREV,
        // right hand
        _______,   _______,  _______,  _______,  _______,  _______,  _______,
        _______,   _______,  _______,  _______,  _______,  _______,  _______,
        _______,   _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  TMUX_LAST_SESSION,
        _______,  _______,
        TMUX_TAB_NEXT
    ),

    [_JITSI] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        /* gets rid of space */       _______,  _______,  _______,  _______,
        /* KC_TAB,   _______, */
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_CLICKUP] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_MATTERHORN] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  MATTERHORN_PREV,
        _______,  _______,  _______,  _______,  _______,  _______,  MATTERHORN_NEXT,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        KC_NO,    _______,  _______,  _______,  _______,  _______,  _______,
        KC_NO,    _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_INKSCAPE] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_N,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_GAMING] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  KC_R,     KC_W,     KC_E,     _______,  _______,
        _______,  KC_G,     KC_A,     KC_S,     KC_D,     KC_F,     TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  KC_SPACE,
                                                          _______,  _______,
                                                          _______,
        // right hand
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                      _______,  _______,  _______,  _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_FN] = LAYOUT_5x7_1(
        // left hand
        QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    TO_DEFAULT_LAYER,
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    _______,
        _______,  _______,  _______,  KC_UP,    _______,  _______,  TG(_GAMING),
        _______,  _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  _______,  TERM_KILL_RUN,
        KC_MSTP,  KC_MPLY,  KC_MPRV,  KC_MNXT,
                                                                 _______, _______,
                                                                 _______, _______,
                                                                 _______, _______,
                                                                 _______,
        // right hand
        KC_F11,   KC_F6,       KC_F7,      KC_F8,    KC_F9,       KC_F10,    QK_BOOT,
        KC_F12,   KC_F6,       KC_F7,      KC_F8,    KC_F9,       KC_F10,    _______,
        KC_F13,   KC_MS_BTN4,  _______,    KC_UP,    KC_MS_BTN5,  _______,   _______,
        KC_F14,   _______,     KC_LEFT,    KC_DOWN,  KC_RGHT,     _______,   _______,
                                           KC_F21,   KC_F22,      KC_F23,    KC_F24,
        _______,  _______,
        KC_DEL,   _______,
        _______,  _______,
        _______
    ),

    [_NUMPAD] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
                                               _______,  _______,
                                               _______,  _______,
                                               _______,  _______,
                                               _______,
        // right hand
        KC_CALC,       _______,  _______,  _______,  _______,  KC_PPLS,  _______,
        SELECT_ALL,    KC_6,     KC_P7,    KC_P8,    KC_P9,    KC_PMNS,  _______,
        COPY,          _______,  KC_P4,    KC_P5,    KC_P6,    KC_PAST,  _______,
        PASTE, _______,  KC_P1,    KC_P2,    KC_P3,    KC_PSLS,  _______,
                                           KC_P0,    KC_PDOT,  KC_PENT,  _______,
        _______,  KC_PENT,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_CMD] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,   _______,   _______,   _______,   _______,  TO_DEFAULT_LAYER,
        _______,  _______,   _______,   _______,   _______,   _______,  _______,
        _______,  _______,   _______,   _______,   _______,   _______,  _______,
        _______,  _______,   _______,   _______,   _______,   _______,  _______,
        MO(_CMD), _______,   _______,   _______,
                                                                  _______, _______,
                                                                  _______, _______,
                                                                  _______, _______,
                                                                  _______,
        // right hand
        _______,  _______,     _______,   _______,   _______,     _______,   _______,
        _______,  _______,     _______,   _______,   _______,     _______,   _______,
        _______,  KC_MS_BTN4,  _______,   _______,   KC_MS_BTN5,  _______,   _______,
        _______,  _______,     _______,   _______,   _______,     _______,   _______,
                                                                  _______,   _______,     _______,   _______,
        _______,  _______,
        _______,  _______,
        _______,  _______,
        _______
    ),

    [_MOUSE] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,     _______,     _______,      _______,  TO_DEFAULT_LAYER,
        _______,  _______,  KC_MS_BTN3,  KC_MS_BTN2,  KC_MS_BTN1,   _______,  _______,
        _______,  _______,  _______,     KC_MS_UP,    _______,      _______,  _______,
        _______,  _______,  KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT,  _______,  _______,
        _______,  _______,  _______,     _______,
                                                                    _______,  _______,
                                                                    _______,  _______,
                                                                    _______,  _______,
                                                                    _______,
        _______,  _______,     _______,     _______,     _______,      _______,  _______,
        _______,  _______,     KC_MS_BTN1,  KC_MS_BTN2,  KC_MS_BTN3,   _______,  _______,
        _______,  KC_MS_BTN4,  _______,     KC_MS_UP,    KC_MS_BTN5,   _______,  _______,
        _______,  _______,     KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT,  _______,  _______,
                                            _______,     _______,      _______,  _______,
        _______,  _______,
        _______,  _______,
        TT(_FN),    TT(_NUMPAD),
        _______
    ),

    [_POWERSCROLL] = LAYOUT_5x7_1(
        // left hand
        _______,  _______,  _______,  _______,  _______,  _______,  TO_DEFAULT_LAYER,
        _______,  _______,  _______,  _______,  _______,  _______,  QK_DYNAMIC_MACRO_RECORD_START_1,
        _______,  _______,  _______,  _______,  _______,  _______,  QK_DYNAMIC_MACRO_RECORD_STOP,
        _______,  _______,  _______,  _______,  _______,  _______,  QK_DYNAMIC_MACRO_PLAY_1,
        _______,  _______,  _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,  _______,
                                                          _______,
        // right hand
        QK_DYNAMIC_MACRO_RECORD_START_2,  _______,  _______,  _______,  _______,  _______,  _______,
        QK_DYNAMIC_MACRO_RECORD_STOP,     _______,  _______,  _______,  _______,  _______,  _______,
        QK_DYNAMIC_MACRO_PLAY_2,          _______,  _______,  _______,  _______,  _______,  _______,
        _______,                          _______,  _______,  _______,  _______,  _______,  _______,
                                                              _______,  _______,  _______,  _______,
        _______,        _______,
        _______,        _______,
        TD(TD_TG_VIM),  _______,
        _______
    ),

};

#if defined(ENCODER_MAP_ENABLE)
                          // left                                        // right
#define DEFAULT_ENCODER { ENCODER_CCW_CW(KC_MS_WH_LEFT, KC_MS_WH_RIGHT), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  }
#define TRANS_ENCODER { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______)  }

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY]      = DEFAULT_ENCODER,
    /* [_MAC]         = TRANS_ENCODER, */
    [_NEOVIM]      = TRANS_ENCODER,
    [_CHROME]      = TRANS_ENCODER,
    [_TERMINAL]    = TRANS_ENCODER,
    [_JITSI]       = TRANS_ENCODER,
    [_CLICKUP]     = TRANS_ENCODER,
    [_MATTERHORN]  = TRANS_ENCODER,
    [_INKSCAPE]    = TRANS_ENCODER,
                       // left                                            // right
    [_NUMPAD]      = { ENCODER_CCW_CW(KC_UP, KC_DOWN),                    ENCODER_CCW_CW(KC_LEFT, KC_RIGHT) },
    [_FN]          = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),                  ENCODER_CCW_CW(TAB_L, TAB_R) },
    [_CMD]         = { ENCODER_CCW_CW(POWERSCROLL_UP, POWERSCROLL_DOWN),  ENCODER_CCW_CW(KC_BSPC, KC_SPC) },
    [_MOUSE]       = { ENCODER_CCW_CW(KC_MS_UP, KC_MS_DOWN),              ENCODER_CCW_CW(KC_MS_LEFT, KC_MS_RIGHT) },
    [_POWERSCROLL] = { ENCODER_CCW_CW(KC_MS_UP, KC_MS_DOWN),              ENCODER_CCW_CW(POWERSCROLL_UP, POWERSCROLL_DOWN) },
};
#endif

#if defined(LEADER_ENABLE)
#define PW(...) ( __VA_ARGS__ SS_TAP(X_ENTER) )// accepts up to 5 keys
KeyCombo leader_keys[] = {
    { KEYS(KC_U, KC_A), USERNAME1 },
    { KEYS(KC_U, KC_S), USERNAME2 },
    { KEYS(KC_U, KC_D), USERNAME3 },
    { KEYS(KC_P, KC_A), PW(PASSWORD1) },
    { KEYS(KC_P, KC_S), PW(PASSWORD2) },
    { KEYS(KC_P, KC_D), PW(PASSWORD2) },
    { KEYS(KC_P, KC_B, KC_W), PW(BITWARDENPW) },
    { KEYS(KC_J, KC_J), "/msg @edjimucator\n" },
    { KEYS(KC_S, KC_R), "/join ~shiny-rocks\n" },
    { KEYS(KC_D, KC_P), "/join ~dustin-private\n" },
    { KEYS(KC_J, KC_I), SS_TAP(X_HOME) "https://jitsi.mulletware.io/\n" },
    { KEYS(KC_1), "!-1:" },
    { KEYS(KC_1, KC_1), "!-1:1" SS_TAP(X_TAB) },
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

CustomModMap CUSTOM_MOD_TAP_KEYS[] = {
    { TERM_TOGGLE, KC_LGUI },
    { TMUX_LEADER, KC_LCTL },
    { CAPS_WORD_TOGGLE, KC_LSFT },
    { TOGGLE_MOUSE, KC_RSFT },
    { CHROME_URL_BAR, KC_LCTL },
    { TAB_EXTRACT, KC_RALT },
    { CTRL_C, KC_RALT },
    { CTRL_K, KC_LALT },
    { TMUX_LAST_SESSION, KC_RALT },
    { TMUX_SESSIONX, KC_LALT },
    { COLON, KC_LALT },
    { VIM_BUFFERS, KC_LCTL },
    { RESUME_SEARCH, KC_RALT }
};

bool process_custom_mod_tap_user(uint16_t keycode, keyrecord_t *record) {
    bool found_key = false;

    for (size_t i = 0; i < sizeof(CUSTOM_MOD_TAP_KEYS) / sizeof(CUSTOM_MOD_TAP_KEYS[0]); ++i) {
        if (CUSTOM_MOD_TAP_KEYS[i].keycode == keycode) {
            found_key = true;

            if (record->event.pressed) {
                on_press(keycode, CUSTOM_MOD_TAP_KEYS[i].mod_code);
                return false;
            } else {
                return on_release(keycode, CUSTOM_MOD_TAP_KEYS[i].mod_code);
            }
        }
    }

    if (!found_key && record->event.pressed) on_other_press();

    return true;
}

KeyCallback CUSTOM_KEYS[] = {
    { DBL_SPACE, &double_space },
    { TOGGLE_VIM, &toggle_vim },
    { CLICK_THIS_SPOT, &click_this_spot },
    { WORKSPACE_LEFT, &workspace_left },
    { WORKSPACE_RIGHT, &workspace_right },
    { POWERSCROLL_UP, &powerscroll_up },
    { POWERSCROLL_DOWN, &powerscroll_down },
    { TO_DEFAULT_LAYER, &to_default_layer },
    { TERM_TOGGLE, &term_toggle },
    { KILL_RUN, &kill_run },
    { TERM_KILL_RUN, &term_kill_run },
    { TMUX_LEADER, &tmux_leader },
    { TMUX_TAB_NEXT, &tmux_tab_next },
    { TMUX_TAB_PREV, &tmux_tab_prev },
    { TMUX_LAST_SESSION, &tmux_last_session },
    { TMUX_SESSIONX, &tmux_sessionx },
    { CAPS_WORD_TOGGLE, &activate_caps_word_toggle },
    { TOGGLE_MOUSE, &toggle_mouse_layer },
    { CHROME_URL_BAR, &chrome_url_bar },
    { TAB_EXTRACT, &tab_extract_combine },
    { SELECT_ALL, &select_all },
    { COPY, &copy },
    { PASTE, &paste },
    { COLON, &colon },
    { VIM_BUFFERS, &vim_buffers },
    { RESUME_SEARCH, &resume_search },
    { CTRL_C, &ctrl_c },
    { CTRL_K, &ctrl_k },
    { CTRL_W, &ctrl_w },
    { MATTERHORN_PREV, &matterhorn_prev },
    { MATTERHORN_NEXT, &matterhorn_next }
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

extern void check_vim_lazy_insert(bool pressed);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

#ifdef CONSOLE_ENABLE // Logging
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    check_vim_lazy_insert(record->event.pressed);

    if (!process_vim_mode(keycode, record)) return false;

    if (process_custom_mod_tap_user(keycode, record)) {
        process_custom_key_user(keycode, record);
    }


    return true;
}


// Declare the detected_os variable at the top of the file for global access
os_variant_t detected_os;

// mac mode for vim and auto-select mac layer
bool process_detected_host_os_kb(os_variant_t os) {
    detected_os = os; // Hoist the value to the global variable

    if (!process_detected_host_os_user(detected_os)) {
        return false;
    }

    if (detected_os == OS_MACOS) {
        enable_vim_for_mac();
        /* set_single_default_layer(_MAC); */
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

void raw_hid_receive(uint8_t *data, uint8_t length) {
    hid_notify(data, length);
}
