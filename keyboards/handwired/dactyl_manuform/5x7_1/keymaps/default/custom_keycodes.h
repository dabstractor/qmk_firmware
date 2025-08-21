#pragma once
#include QMK_KEYBOARD_H

// Shared custom keycodes enum so multiple compilation units can reference them

enum custom_keycodes {
    DBL_SPACE = SAFE_RANGE,
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
    TMUX_SESSIONX,
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
};
