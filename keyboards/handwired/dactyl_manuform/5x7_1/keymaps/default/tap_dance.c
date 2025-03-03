#include QMK_KEYBOARD_H
#include "./qmk-vim/src/vim.h"
#include "leader.h"

enum {
    TD_LEAD_FN,
    TD_TG_VIM
};

void fn_layer_dance(tap_dance_state_t *state, callback_t fn) {
    if(state->pressed) {
        layer_on(_FN);
    } else {
        fn();
    }
}

void fn_layer_reset(tap_dance_state_t *state, void *user_data) {
    layer_off(_FN);
}

void qmk_leader(void) {
    leader_sequence_active() ? leader_end() : leader_start();
}

void leader_fn_each(tap_dance_state_t *state, void *user_data) {
    fn_layer_dance(state, &qmk_leader);
}

void vim_fn_each(tap_dance_state_t *state, void *user_data) {
    fn_layer_dance(state, &toggle_vim_mode);
}


tap_dance_action_t tap_dance_actions[] = {
    // press and release for leader key, hold for fn layer
    [TD_LEAD_FN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, &leader_fn_each, &fn_layer_reset),
    [TD_TG_VIM]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, &vim_fn_each, &fn_layer_reset)
};
