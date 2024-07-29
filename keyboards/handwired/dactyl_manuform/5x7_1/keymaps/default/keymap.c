#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _FN     1
#define _NUMPAD 2
#define _CMD 3

// Some basic macros
#define TASK   LCTL(LSFT(KC_ESC))
#define TAB_R  LCTL(KC_TAB)
#define TAB_L  LCTL(LSFT(KC_TAB))
#define TAB_RO LCTL(LSFT(KC_T))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x7_1(
        // left hand
        KC_GRV,        KC_1,     KC_2,    KC_3,   KC_4,   KC_5,   KC_NO,
        KC_TAB,        KC_Q,     KC_W,    KC_E,   KC_R,   KC_T,   TAB_L,
        KC_ESC,        KC_A,     KC_S,    KC_D,   KC_F,   KC_G,   KC_LBRC,
        KC_LSFT,       KC_Z,     KC_X,    KC_C,   KC_V,   KC_B,   TAB_L,
        KC_CAPS,       TG(_CMD), KC_LBRC, KC_RBRC,
                                                          OSL(_CMD),  KC_LCTL,
                                                          KC_LALT, KC_LGUI,
                                                          TT(_FN), TT(_NUMPAD),
        // right hand
        TG(_NUMPAD),   KC_6,    KC_7,   KC_8,     KC_9,     KC_0,     KC_LBRC,
        TAB_R,         KC_Y,    KC_U,   KC_I,     KC_O,     KC_P,     KC_RBRC,
        KC_RBRC,       KC_H,    KC_J,   KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        TAB_R,         KC_N,    KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
                                        KC_MINS,  KC_EQL,   KC_BSLS,  KC_NO,
        KC_BSPC, KC_SPC,
        KC_ENT,  KC_RCTL,
        KC_RGUI, KC_RALT
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
                                                               _______, TT(_CMD),

        // right hand
        _______,   KC_F6,     KC_F7,      KC_F8,    KC_F9,     KC_F10,    _______,
        KC_F12,    _______,   _______,    KC_UP,    _______,   _______,   _______,
        _______,   _______,   KC_LEFT,    KC_DOWN,  KC_RGHT,   _______,   _______,
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
                                                               _______,  _______,
                                                               _______,  _______,
                                                               TT(_CMD), _______,

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

    [_CMD] = LAYOUT_5x7_1(
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
        _______,   _______,   _______,   _______,   _______,   _______,   _______,
        _______,   _______,   _______,   _______,   _______,   _______,   _______,
        _______,   _______,   _______,   _______,   _______,   _______,   _______,
        _______,   _______,   _______,   _______,   _______,   _______,   _______,
                                         _______,   _______,   _______,   _______,
        _______,   _______,
        _______,   _______,
        _______,   _______
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [_FN]     = { ENCODER_CCW_CW(KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP),  ENCODER_CCW_CW(KC_MS_WH_LEFT, KC_MS_WH_RIGHT)  },
    [_NUMPAD] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
    [_CMD] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif

// #if defined(DIP_SWITCH_MAP_ENABLE)
// const uint16_t PROGMEM dip_switch_map[NUM_DIP_SWITCHES][NUM_DIP_STATES] = {
//
//     // DIP_SWITCH_OFF_ON(
//     //     tap_code(KC_A),
//     //     tap_code(KC_B)
//     // ),
//     // DIP_SWITCH_OFF_ON(
//     //     tap_code(KC_A),
//     //     tap_code(KC_B)
//     // )
//     // DIP_SWITCH_OFF_ON(DF(0), DF(1))
//     DIP_SWITCH_OFF_ON(MO(_NUMPAD), MO(_QWERTY))
//     // DIP_SWITCH_OFF_ON(EC_NORM, EC_SWAP)
// };
// #endif

// bool dip_switch_update_user(uint8_t index, bool active) {
//     if (is_keyboard_master()) {
//         switch(index) {
//             case 0: // _QWERTY layer
//                 if(active) {
//                     tap_code(KC_A);
//                     // tap_code(KC_MS_WH_RIGHT);
//                 } else {
//                     // nothing
//                 }
//         }
//     } else {
//         // not being detected
//     }
//         switch(index) {
//             case 0: // _QWERTY layer
//                 if(active) {
//                     tap_code(KC_B);
//                     // tap_code(TAB_L);
//                 } else {
//
//                 }
//             break;
//         }
//     // Debugging output
//     uprintf("DIP switch %d %s\n", index, active ? "active" : "inactive");
//
//     return true;
// }

// void keyboard_pre_init_user(void) {
//     // Enable pull-up resistor on all DIP switch pins
//     setPinInputHigh(GP27);
// }

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   // If console is enabled, it will print the matrix position and status of each key pressed
// #ifdef CONSOLE_ENABLE
//     uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
// #endif
//   return true;
// }

