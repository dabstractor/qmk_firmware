#pragma once

#include QMK_KEYBOARD_H

// Layer-pair encoder overlays
// ---------------------------------------------------------------------------
// encoder_map[] alone cannot vary a utility layer's (_FN/_NUMPAD/_CMD/_MOUSE)
// encoder behavior per application layer, because those layers sit on top of
// the QMKonnect app layers and always win the stock transparency walk. This
// table adds a second dimension: [context][top] = "context layer's version of
// the top layer's encoders" — e.g. [_NEOVIM][_FN] is "neovim's FN knob".
//
// Semantics (see layer-pair-overlays-research.md §10):
//   * Unwritten pair (or 0 cell)  -> stock encoder_map behavior. Today's
//     behavior is the default; every pair you don't mention is unchanged.
//   * `_______` (KC_TRNS) in a slot -> that encoder/direction explicitly
//     inherits stock even inside a written row.
//   * Any keycode form works: basic, modded (LCTL(KC_TAB)), keymap macros
//     (TAB_L), and QK_KB custom keycodes (routed to CUSTOM_KEYS/mod-tap
//     dispatchers as usual, so callbacks work per-pair for free).
//   * Dispatch walks the ACTIVE layer stack below the top layer, high->low,
//     probing pair[context][top] until the first non-zero/non-TRNS hit. So
//     [_QWERTY][_FN] acts as a blanket default inherited by every app layer
//     without its own [_FN] entry, and tri-layer _CMD (which sits above still-
//     active _FN/_NUMPAD bits) resolves to [_NEOVIM][_CMD] correctly.
//
// Storage is a flat [32][32][NUM_ENCODERS][NUM_DIRECTIONS] table (8 KiB flash
// on this ARM build); the macro is the interface — storage can go sparse later
// without touching this syntax.

#define ENCODER_PAIR_MAX_LAYERS 32 // == LAYER_STATE_32BIT width; layers.c tops out at _MOUSE=31

#define DEFINE_ENCODER_PAIRS(...)                                              \
    const uint16_t PROGMEM                     \
        encoder_pair_map[ENCODER_PAIR_MAX_LAYERS][ENCODER_PAIR_MAX_LAYERS][NUM_ENCODERS][NUM_DIRECTIONS] = { __VA_ARGS__ }

// Declared here so the dispatcher (implementation) can reference the table no
// matter where in the translation unit the definition (usage) appears — which,
// by keymap convention, is in keymap.c next to CUSTOM_KEYS/CUSTOM_MOD_TAP_KEYS.
extern const uint16_t encoder_pair_map[ENCODER_PAIR_MAX_LAYERS][ENCODER_PAIR_MAX_LAYERS][NUM_ENCODERS][NUM_DIRECTIONS];

bool encoder_pairs_process(uint16_t keycode, keyrecord_t *record);
