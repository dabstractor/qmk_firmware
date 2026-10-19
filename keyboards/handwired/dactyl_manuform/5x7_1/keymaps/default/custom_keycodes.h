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
    VIM_RESUME_SEARCH,
    VIM_COMMAND,
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
    MOUSE_TO_CORNER,
    VLC_START_STOP
};
