#include QMK_KEYBOARD_H
#include "./encoder_pairs.h"

// Implementation only — the pair table (usage) is defined in keymap.c, next to
// encoder_map / CUSTOM_KEYS / CUSTOM_MOD_TAP_KEYS, per keymap convention.

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)

// Runs from process_record_user, immediately after the powerscroll passthrough
// (so the x10/x100 multiplier still wins while POWERSCROLL_HOLD is down).
// Returns false when the event was claimed as a pair action (both the press
// and release halves of the synthetic encoder keydown/keyup pair are consumed,
// exactly like powerscroll consumes encoder events).
bool encoder_pairs_process(uint16_t keycode, keyrecord_t *record) {
    (void)keycode;

    if (!IS_ENCODEREVENT(record->event)) {
        return true; // not ours
    }

    layer_state_t remaining = layer_state | default_layer_state;
    if (!remaining) {
        return true;
    }

    const uint8_t enc = record->event.key.col;
    const bool    cw  = (record->event.key.row == KEYLOC_ENCODER_CW);

    // `top` stays fixed at the highest ACTIVE layer — that is "which overlay
    // context you're in" (e.g. _CMD when both thumb keys are held, since
    // tri-layer keeps the _FN/_NUMPAD bits set below it). The *context* walks
    // down the remaining active layers so utility-layer intermediates are
    // skipped on the way to the app layer: holding _CMD over neovim probes
    // [_FN][_CMD], [_NUMPAD][_CMD], then [_NEOVIM][_CMD] — first hit wins.
    // Walking `top` itself down instead would wrongly fire [_NEOVIM][_FN]
    // while _CMD is held; don't.
    uint8_t  top    = get_highest_layer(remaining);
    uint16_t action = 0;

    while (remaining) {
        layer_state_t rest = remaining & ~((layer_state_t)1 << top);
        if (!rest) break; // nothing beneath `top` — no pair possible

        uint8_t context = get_highest_layer(rest);
        action = encoder_pair_map[context][top][enc][cw ? 0 : 1]; // index 0 = CW, 1 = CCW (matches encoder_map layout)
        if (action && action != KC_TRNS) break;

        // Clear the context bit — NOT `remaining = rest`. rest still contains
        // the context bit, so assigning it verbatim makes no progress once the
        // walk reaches the base layer and the probe keeps missing: infinite
        // loop, dead keyboard. Each iteration must remove exactly one bit.
        remaining = rest & ~((layer_state_t)1 << context);
    }

    // 0 = unwritten pair, KC_TRNS = explicit inherit: either way, fall through
    // to the stock encoder_map dispatch (untouched).
    if (!action || action == KC_TRNS) {
        return true;
    }

#ifdef CONSOLE_ENABLE
    uprintf("ENCODER_PAIRS: ctx/top hit kc=0x%04X pressed=%d\n", action, record->event.pressed);
#endif

    // tap_code16 emits the literal table value — modded keycodes, macros and
    // QK_KB customs all work; no powerscroll-style keycode offset applies.
    if (record->event.pressed) {
        tap_code16(action);
    }
    return false; // consume this half (press or release)
}

#else

bool encoder_pairs_process(uint16_t keycode, keyrecord_t *record) {
    (void)keycode;
    (void)record;
    return true;
}

#endif
