#pragma once
#include "./qmk-vim/src/vim.h"

enum custom_keycodes {
    DBL_SPACE = SAFE_RANGE,
    CLICK_THIS_SPOT,
    TOGGLE_VIM,
    WORKSPACE_LEFT,
    WORKSPACE_RIGHT,
    POWERSCROLL_UP,
    POWERSCROLL_DOWN,
    TERM_TOGGLE,
    CAPS_WORD_TOGGLE,
    TMUX_LEADER,
    TOGGLE_MOUSE,
    COLON,
    TO_DEFAULT_LAYER,
    TOGGLE_MAC,
    TOGGLE_COLEMAK
};

void double_space(uint16_t keycode, keyrecord_t *record) {
    send_string("  ");
}

void toggle_vim(uint16_t keycode, keyrecord_t *record) {
    toggle_vim_mode();
}

void click_this_spot(uint16_t keycode, keyrecord_t *record) {
    tap_code(KC_MS_LEFT);
    tap_code(KC_MS_UP);
    tap_code(KC_MS_RIGHT);
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

void to_default_layer(uint16_t keycode, keyrecord_t *record) {
    layer_move(biton32(default_layer_state));
    disable_vim_mode();
}

void toggle_default_layer(int layer) {
    set_single_default_layer(default_layer_state == (1UL << layer) ? _QWERTY : layer);
}

void toggle_macos_default(uint16_t keycode, keyrecord_t *record) {
    toggle_default_layer(_MAC);
}

void toggle_colemak_default(uint16_t keycode, keyrecord_t *record) {
    toggle_default_layer(_COLEMAK);
}

#define POWERSCROLL_FACTOR 15

void repeat_code(uint16_t keycode, uint8_t count) {
    for(int i = 0; i < count; i++) {
        register_code(keycode);
        unregister_code(keycode);
    }
}

void powerscroll_up(uint16_t keycode, keyrecord_t *record) {
    repeat_code(KC_MS_WH_UP, POWERSCROLL_FACTOR);
}

void powerscroll_down(uint16_t keycode, keyrecord_t *record) {
    repeat_code(KC_MS_WH_DOWN, POWERSCROLL_FACTOR);
}

void term_toggle(uint16_t keycode, keyrecord_t *record) {
    // send GUI+Space
    register_code(KC_RALT);
    register_code(KC_SPC);
    unregister_code(KC_RALT);
    unregister_code(KC_SPC);

    disable_vim_mode(); // if I'm doing stuff in the terminal, I definitely don't want to be in vim mode anymore
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

void copy(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_LCTL);
    register_code(KC_C);
    unregister_code(KC_LCTL);
    unregister_code(KC_C);
}

void paste(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_LCTL);
    register_code(KC_V);
    unregister_code(KC_LCTL);
    unregister_code(KC_V);
}

void select_all(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_LCTL);
    register_code(KC_A);
    unregister_code(KC_LCTL);
    unregister_code(KC_A);
}

void colon(uint16_t keycode, keyrecord_t *record) {
    register_code(KC_LSFT);
    register_code(KC_SCLN);
    unregister_code(KC_LSFT);
    unregister_code(KC_SCLN);
}
