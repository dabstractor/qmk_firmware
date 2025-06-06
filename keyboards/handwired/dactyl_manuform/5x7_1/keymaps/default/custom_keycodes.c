#pragma once
#include QMK_KEYBOARD_H
#include "./qmk-vim/src/vim.h"
#include "./qmk-vim/src/modes.h"
#include "keycombo.h"

extern os_variant_t detected_os;

enum custom_keycodes {
    DBL_SPACE = SAFE_RANGE,
    CLICK_THIS_SPOT,
    TOGGLE_VIM,
    WORKSPACE_LEFT,
    WORKSPACE_RIGHT,
    POWERSCROLL_UP,
    POWERSCROLL_DOWN,
    TERM_TOGGLE,
    KILL_RUN,
    TERM_KILL_RUN,
    CAPS_WORD_TOGGLE,
    TMUX_LEADER,
    TMUX_TAB_NEXT,
    TMUX_TAB_PREV,
    TMUX_LAST_SESSION,
    TMUX_SESSIONX,
    VIM_BUFFERS,
    TOGGLE_MOUSE,
    COLON,
    TO_DEFAULT_LAYER,
    CHROME_URL_BAR,
    TAB_EXTRACT,
    CTRL_C,
    CTRL_K,
    CTRL_W,
    MATTERHORN_PREV,
    MATTERHORN_NEXT,
};

void press(uint16_t key) {
    register_code(key);
    unregister_code(key);
}

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
    combo(KC_LGUI, KC_H);
}

void workspace_right(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LGUI, KC_L);
}

void to_default_layer(uint16_t keycode, keyrecord_t *record) {
    layer_move(biton32(default_layer_state));
    disable_vim_mode();
}

void toggle_default_layer(int layer) {
    set_single_default_layer(default_layer_state == (1UL << layer) ? _QWERTY : layer);
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
    combo(KC_LALT, KC_SPC);
    disable_vim_mode(); // if I'm doing stuff in the terminal, I definitely don't want to be in vim mode anymore
}

void tmux_leader(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_SPC);
}

void vim_buffers(uint16_t keycode, keyrecord_t *record) {
    press(KC_SPC);
    press(KC_SPC);
}

void activate_caps_word_toggle(uint16_t keycode, keyrecord_t *record) {
    caps_word_toggle();
}

void toggle_mouse_layer(uint16_t keycode, keyrecord_t *record) {
    layer_invert(_MOUSE);
}

void copy(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_C);
}

void paste(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_V);
}

void select_all(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_A);
}

void colon(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LSFT, KC_SCLN);
}

void chrome_url_bar(uint16_t keycode, keyrecord_t *record) {
    if (detected_os == OS_MACOS) {
        combo(KC_LGUI, KC_L);
        return;
    }

    combo(KC_LCTL, KC_L);
}

void tab_extract(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LALT, KC_LSFT, KC_X);
}

void tab_combine(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LALT, KC_LSFT, KC_Z);
}

void tab_extract_combine(uint16_t keycode, keyrecord_t *record) {
    if (get_mods() & MOD_MASK_ALT) {
        unregister_code(KC_LALT);
        tab_combine(keycode, record);
        register_code(KC_LALT);
    } else {
        tab_extract(keycode, record);
    }
}

void tmux_tab_next(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_SPC);
    press(KC_N);
}

void tmux_tab_prev(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_SPC);
    press(KC_P);
}

void tmux_last_session(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_SPC);
    combo(KC_LSFT, KC_L);
}

void tmux_sessionx(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_SPC);
    press(KC_O);
}

void ctrl_c(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_C);
}

void ctrl_k(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_K);
}

void ctrl_w(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_W);
}

void matterhorn_prev(uint16_t keycode, keyrecord_t *record) {
    SEND_STRING("/left\n");
}

void matterhorn_next(uint16_t keycode, keyrecord_t *record) {
    SEND_STRING("/right\n");
}

void kill_run(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LCTL, KC_C);
    combo(KC_LCTL, KC_K);
    press(KC_ENTER);
}

void term_kill_run(uint16_t keycode, keyrecord_t *record) {
    bool is_layer_off = IS_LAYER_OFF(_TERMINAL);

    if (is_layer_off) {
        term_toggle(keycode, record);
    }

    bool is_vim_enabled = vim_mode_enabled();
    vim_mode_t vim_mode = get_vim_mode();
    wait_ms(100);
    kill_run(keycode, record);

    if (is_vim_enabled) {
        enable_vim_mode();
        switch (vim_mode) {
            case NORMAL_MODE:
                normal_mode();
                break;
            case INSERT_MODE:
                insert_mode();
                break;
            case VISUAL_MODE:
                visual_mode();
                break;
            case VISUAL_LINE_MODE:
                visual_line_mode();
                break;
            default:
                break;
        }
    }

    if (is_layer_off) {
        term_toggle(keycode, record);
    }
}
