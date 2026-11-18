#pragma once
#include QMK_KEYBOARD_H

// Shared custom keycodes enum so multiple compilation units can reference them
//
// VIA note: rebased to QK_KB_0 (0x7E00) instead of SAFE_RANGE (QK_USER,
// 0x7E40). The VIA app has only a QK_KB constant (0x7E00) — no QK_USER — so its
// definition `customKeycodes` array is indexed from 0x7E00. Starting this enum
// there makes each keycode line up with its label in via_definition.json.
// (default keymap is unchanged — this rebasing is via-only.)
enum custom_keycodes {
    DBL_SPACE = QK_KB_0,
    CLICK_THIS_SPOT,
    TOGGLE_VIM,
    WORKSPACE_LEFT,
    WORKSPACE_RIGHT,
    POWERSCROLL_HOLD,
    POWERSCROLL,
    TERM_TOGGLE,
    KILL_RUN,
    TERM_KILL_RUN,
    CAPS_WORD_TOGGLE,
    TMUX_LEADER,
    TMUX_TAB_NEXT,
    TMUX_TAB_PREV,
    TMUX_LAST_SESSION,
    TMUX_ZOOM,
    TMUX_COPY_MODE,
    TMUX_SESSIONX,
    TMUX_HISTORY_BACK,
    TMUX_HISTORY_FORWARD,
    VIM_BUFFERS,
    VIM_TAB_LEADER,
    VIM_COMMAND,
    TOGGLE_MOUSE,
    COLON,
    CHROME_URL_BAR,
    TAB_EXTRACT,
    CTRL_C,
    CTRL_K,
    CTRL_W,
    MATTERHORN_PREV,
    MATTERHORN_NEXT,
    MOUSE_TO_CORNER,
    VLC_START_STOP
};

// ---- Handler declarations --------------------------------------------------
// Every handler, declared once, here — the one canonical surface. Any
// compilation unit (keymap.c, super_leader.def, leader_layers.def) can
// reference any handler by name with zero glue: no local forward-decl
// shims, no commented-out prototypes, no "prototype is local because…".
void double_space(uint16_t keycode, keyrecord_t *record);
void toggle_vim(uint16_t keycode, keyrecord_t *record);
void click_this_spot(uint16_t keycode, keyrecord_t *record);
void mouse_to_corner(uint16_t keycode, keyrecord_t *record);
void workspace_left(uint16_t keycode, keyrecord_t *record);
void workspace_right(uint16_t keycode, keyrecord_t *record);
void to_default_layer(uint16_t keycode, keyrecord_t *record);
void term_toggle(uint16_t keycode, keyrecord_t *record);
void vim_buffers(uint16_t keycode, keyrecord_t *record);
void activate_caps_word_toggle(uint16_t keycode, keyrecord_t *record);
void toggle_mouse_layer(uint16_t keycode, keyrecord_t *record);
void copy(uint16_t keycode, keyrecord_t *record);
void paste(uint16_t keycode, keyrecord_t *record);
void select_all(uint16_t keycode, keyrecord_t *record);
void colon(uint16_t keycode, keyrecord_t *record);
void chrome_url_bar(uint16_t keycode, keyrecord_t *record);
void tab_extract(uint16_t keycode, keyrecord_t *record);
void tab_combine(uint16_t keycode, keyrecord_t *record);
void tab_extract_combine(uint16_t keycode, keyrecord_t *record);
void tmux_leader(uint16_t keycode, keyrecord_t *record);
void tmux_last_window(uint16_t keycode, keyrecord_t *record);
void tmux_last_session(uint16_t keycode, keyrecord_t *record);
void tmux_last_pane(uint16_t keycode, keyrecord_t *record);
void tmux_win_prev(uint16_t keycode, keyrecord_t *record);
void tmux_tab_next(uint16_t keycode, keyrecord_t *record);
void tmux_tab_prev(uint16_t keycode, keyrecord_t *record);
void tmux_zoom(uint16_t keycode, keyrecord_t *record);
void tmux_sessionx(uint16_t keycode, keyrecord_t *record);
void tmux_copy_mode(uint16_t keycode, keyrecord_t *record);
void tmux_history_back(uint16_t keycode, keyrecord_t *record);
void tmux_history_forward(uint16_t keycode, keyrecord_t *record);
void tmux_yank_current_line(uint16_t keycode, keyrecord_t *record);
void tmux_search_reverse(uint16_t keycode, keyrecord_t *record);
void ctrl_c(uint16_t keycode, keyrecord_t *record);
void ctrl_k(uint16_t keycode, keyrecord_t *record);
void ctrl_w(uint16_t keycode, keyrecord_t *record);
void matterhorn_prev(uint16_t keycode, keyrecord_t *record);
void matterhorn_next(uint16_t keycode, keyrecord_t *record);
void kill_run(uint16_t keycode, keyrecord_t *record);
void term_kill_run(uint16_t keycode, keyrecord_t *record);
void vim_tab_leader(uint16_t keycode, keyrecord_t *record);
void vim_command(uint16_t keycode, keyrecord_t *record);
void vlc_start_stop(uint16_t keycode, keyrecord_t *record);

// No-arg helper, callable by name from SL_FN/LL_FN like any handler.
void send_tmux_leader(void);

// ---- Leader callback interface ---------------------------------------------
// SL_FN/LL_FN (super_leader.def / leader_layers.def) accept any handler by
// name. LEADER_BRIDGE picks the trampoline matching the handler's signature
// at compile time — a wrong-shaped function fails the build instead of
// silently being called through a mismatched type.
typedef void (*keycode_handler_t)(uint16_t keycode, keyrecord_t *record);
typedef void (*plain_handler_t)(void);

void leader_call_keycode_handler(void *user_data);  // calls fn(0, NULL)
void leader_call_plain_handler(void *user_data);    // calls fn()

#define LEADER_BRIDGE(fn_)                              \
    _Generic((fn_),                                     \
        keycode_handler_t: leader_call_keycode_handler, \
        plain_handler_t:   leader_call_plain_handler)
