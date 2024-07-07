#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _FN     1
#define _NUMPAD 2

// Some basic macros
#define TASK   LCTL(LSFT(KC_ESC))
#define TAB_R  LCTL(KC_TAB)
#define TAB_L  LCTL(LSFT(KC_TAB))
#define TAB_RO LCTL(LSFT(KC_T))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x7_1(
        // left hand
        KC_GRV,        KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_NO,
        KC_TAB,        KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,   KC_LBRC,
        KC_ESC,        KC_A,    KC_S,    KC_D,   KC_F,   KC_G,   TAB_L,
        OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_MS_WH_UP,
        KC_CAPS,       KC_LGUI, TT(_FN), KC_NO,
                                                    KC_SPC, KC_BSPC,
                                                    KC_LCTL, KC_LGUI,
                                                    KC_LALT, TT(_FN),
        // right hand
        TG(_NUMPAD),   KC_6,    KC_7,    KC_8,     KC_9,     KC_0,     KC_BSPC,
        KC_RBRC,       KC_Y,    KC_U,    KC_I,     KC_O,     KC_P,     KC_BSLS,
        TAB_R,         KC_H,    KC_J,    KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_MS_WH_DOWN, KC_N,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  OSM(MOD_RSFT),
                                         KC_MINS,  KC_EQL,   KC_NO,    KC_NO,
        KC_ENT,  KC_SPC,
        KC_RCTL, KC_RGUI,
        TT(_FN), KC_RALT
    ),

    [_FN] = LAYOUT_5x7_1(
        // left hand
        _______,   KC_F1,     KC_F2,      KC_F3,    KC_F4,     KC_F5,    _______,
        _______,   _______,   _______,    KC_UP,    _______,   _______,  KC_F11,
        _______,   _______,   KC_LEFT,    KC_DOWN,  KC_RGHT,   _______,  QK_BOOT,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        KC_MSTP,   KC_MPLY,   KC_MPRV,   KC_MNXT,
                                                               _______, _______,
                                                               _______, _______,
                                                               _______, _______,

        // right hand
          _______,   KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,    _______,
          KC_F12,    _______,   _______,   _______,   _______,   _______,   _______,
          _______,   _______,   _______,   _______,   _______,   _______,   _______,
          _______,   _______,   _______,   _______,   _______,   _______,   _______,
                                           _______,   _______,   _______,   _______,
          KC_DEL,    _______,
          _______,   _______,
          _______,   _______
    ),

    [_NUMPAD] = LAYOUT_5x7_1(
        // left hand
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,
                                                                _______, _______,
                                                                _______, _______,
                                                                _______, _______,

        // right hand
        _______,   _______,   KC_NUM,    _______,   KC_PMNS,   KC_PPLS,   _______,
        _______,   _______,   KC_P7,     KC_P8,     KC_P9,     _______,   _______,
        _______,   _______,   KC_P4,     KC_P5,     KC_P6,     KC_PAST,   _______,
        _______,   _______,   KC_P1,     KC_P2,     KC_P3,     KC_PSLS,   _______,
                                         KC_P0,     KC_PDOT,   _______,   _______,
         _______, KC_PENT,
         _______, _______,
         _______, _______
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [_FN]     = { ENCODER_CCW_CW(KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP),  ENCODER_CCW_CW(KC_MS_WH_LEFT, KC_MS_WH_RIGHT)  },
    [_NUMPAD] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}
