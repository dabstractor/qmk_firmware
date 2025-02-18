#include QMK_KEYBOARD_H
#include "leader.h"

enum {
    TD_LEAD_FN
};

void qmk_leader(void) {
    leader_sequence_active() ? leader_end() : leader_start();
}

void leader_fn_each(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        layer_on(_FN);
    } else {
        qmk_leader();
    }
}

void leader_fn_reset(tap_dance_state_t *state, void *user_data) {
    layer_off(_FN);
}

tap_dance_action_t tap_dance_actions[] = {
    // press and release for leader key, hold for fn layer
    [TD_LEAD_FN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, leader_fn_each, leader_fn_reset)
};
