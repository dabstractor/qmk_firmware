#pragma once
#include QMK_KEYBOARD_H
#include "./qmk-vim/src/vim.h"
#include "./qmk-vim/src/modes.h"
#include "keycombo.h"
#include "utils/mods.c"
#include "./custom_keycodes.h"

extern os_variant_t detected_os;

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

void mouse_to_corner(uint16_t keycode, keyrecord_t *record) {
    // Send the cursor to the bottom-left corner, assuming it starts near the
    // middle of the screen. Each tap is ~16px (MK_C_OFFSET_UNMOD); the counts
    // below (120 down, 180 left) cover up to a 4K display with margin. The
    // moves are interleaved (3 left, 2 down per round) so the cursor travels
    // diagonally rather than down-then-left. The 2ms gap keeps each tap in its
    // own USB report; ~300 taps total runs in ~0.6s.
    //   down: 120 * 16px = 1920px   left: 180 * 16px = 2880px
    uint8_t left = 180;
    uint8_t down = 120;
    while (left > 0 || down > 0) {
        for (uint8_t i = 0; i < 3 && left > 0; i++) {
            tap_code(KC_MS_LEFT);
            wait_ms(2);
            left--;
        }
        for (uint8_t i = 0; i < 2 && down > 0; i++) {
            tap_code(KC_MS_DOWN);
            wait_ms(2);
            down--;
        }
    }
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

void repeat_code(uint16_t keycode, uint8_t count) {
    for(int i = 0; i < count; i++) {
        register_code(keycode);
        unregister_code(keycode);
    }
}

void term_toggle(uint16_t keycode, keyrecord_t *record) {
    combo(KC_LALT, KC_SPC);
    disable_vim_mode(); // if I'm doing stuff in the terminal, I definitely don't want to be in vim mode anymore
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

void send_tmux_leader(void) {
    combo(KC_LCTL, KC_SPC);
}

void tmux_last_window(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LSFT, KC_B);
}

void tmux_last_session(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LSFT, KC_L);
}

void tmux_last_pane(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LSFT, KC_G);
}

void tmux_history_back(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LCTL, KC_F9);
}

void tmux_history_forward(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LCTL, KC_F10);
}

void tab_extract_combine(uint16_t keycode, keyrecord_t *record) {
    IF_MODS_ELSE(
        MOD_MASK_ALT,
        { tab_combine(keycode, record); },
        { tab_extract(keycode, record); }
    );
}

void tmux_win_prev(uint16_t keycode, keyrecord_t *record) {
    int found_key = 1;

    IF_MODS_ELSE(
        MOD_MASK_SHIFT,
        { tmux_last_pane(keycode, record); },
        { found_key = 0; }
    );

    IF_MODS_ELSE(
        MOD_MASK_ALT,
        { tmux_last_window(keycode, record); },
        { found_key = 0; }
    );

    if (!found_key) {
        { tmux_last_session(keycode, record); }
    }

}


void tmux_leader(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
}

void tmux_tab_next(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    press(KC_N);
}

void tmux_tab_prev(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    press(KC_P);
}

void tmux_zoom(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    press(KC_Z);
}

void tmux_sessionx(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    press(KC_O);
}

void tmux_copy_mode(uint16_t keycode, keyrecord_t *record) {
    send_tmux_leader();
    combo(KC_LSFT, KC_LBRC);
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

void vim_resume_search(uint16_t keycode, keyrecord_t *record) {
    IF_MODS_ELSE(
        MOD_MASK_ALT,
        { send_string(" sr"); },
        { ctrl_w(keycode, record); }
    );
}

void vim_command(uint16_t keycode, keyrecord_t *record) {
    IF_MODS_ELSE(
        MOD_MASK_ALT,
        { press(KC_ESC); send_string(" :"); press(KC_UP); },
        { colon(keycode, record); }
    );
}

