// leader_layers.c — machinery behind leader_layers.def. Write once, never
// touch again: the entire user-facing surface lives in leader_layers.def.
//
// How it works (for the curious, not for editing):
//   * Each LL_LAYER block becomes one element of a pointer table indexed by
//     layer number (designated init), pointing at an exactly-sized
//     compound-literal sequence array terminated by a {0} sentinel. On
//     RP2040, PROGMEM/PSTR/pgm_read_* are no-ops and file-scope compound
//     literals have static storage, so entries are self-contained
//     initializers — no symbol names, no introspection passes.
//   * When a leader sequence starts, super_leader_start_user() snapshots the
//     app layer (utility layers _NUMPAD/_FN/_CMD/_MOUSE are masked out, per
//     the layers.c convention, so a held MO or a dance-latched layer doesn't
//     distort it) and binds that layer's block. The layer stays frozen for
//     the sequence's duration — the module ignores layer keys mid-sequence,
//     so "the layer LEADER was pressed on" is the only coherent per-layer
//     definition.
//   * Per-key dispatch rides the module's documented extension hook
//     super_leader_add_user(): after the module matches the global
//     super_leader.def table, this callback matches the bound layer's
//     entries the same way — full match claims the output (called last, so
//     a layer entry shadows a .def entry on an exact key collision),
//     partial match defers resolution for longer layer sequences. All other
//     matching semantics (longest match, immediate resolution, replay of
//     divergent keys, timeout) stay module-native.
//
//     (Why not override the weak super_leader_sequence_count/get() accessors
//     instead? QMK compiles keymap.c into the same translation unit as the
//     module's introspection.c, where those weak defaults live — same-TU
//     redefinition is a hard error. add_user()'s weak default is in
//     super_leader.c, a separate TU, so it links properly.)
#include QMK_KEYBOARD_H
#include "super_leader.h"
#include "custom_keycodes.h"
#include "./layers.c"

#define LEADER_APP_MASK \
    (~(layer_state_t)((1u << _NUMPAD) | (1u << _FN) | (1u << _CMD) | (1u << _MOUSE)))

// Entry wrappers — mirror the SL_* family; each carries its own trailing
// comma so blocks stay comma-free, and LL_LAYER appends the {0} sentinel.
#define LL_KEYS(keys_) (const uint16_t[]){ keys_, SUPER_LEADER_END }
#define LL_STR(keys_, str_) { .keys = LL_KEYS(keys_), .output = SUPER_LEADER_STR(str_) },
#define LL_KEY(keys_, kc_)  { .keys = LL_KEYS(keys_), .output = SUPER_LEADER_KEY(kc_) },
#define LL_UNI(keys_, str_) { .keys = LL_KEYS(keys_), .output = SUPER_LEADER_UNI(str_) },
#define LL_FN(keys_, fn_)   { .keys = LL_KEYS(keys_), .output = SUPER_LEADER_FUN(LEADER_BRIDGE(fn_), fn_) },
#define LL_LAYER(layer_, ...) \
    [layer_] = { (const super_leader_sequence_t[]){ __VA_ARGS__ {0} } },

typedef struct {
    const super_leader_sequence_t* seqs;  // {0}-sentinel-terminated; NULL = no block
} leader_layer_block_t;

static const leader_layer_block_t leader_layer_blocks[_MOUSE + 1] = {
#if __has_include("leader_layers.def")
#include "leader_layers.def"
#endif
};

// The block bound at sequence start.
static const super_leader_sequence_t* bound_seqs;
static uint8_t bound_seq_count;

void super_leader_start_user(void) {
    bound_seqs = leader_layer_blocks[get_highest_layer(layer_state & LEADER_APP_MASK)].seqs;
    for (bound_seq_count = 0; bound_seqs && bound_seqs[bound_seq_count].keys; ++bound_seq_count) {
    }
}

// Module-idiomatic match: does the typed prefix `seq` (length num_seq) fully
// match this entry, or is it a proper prefix of it (keep waiting)?
static bool ll_entry_matches(const super_leader_sequence_t* entry, const uint16_t* seq, uint8_t num_seq, bool* partial) {
    for (uint8_t i = 0; i < num_seq; ++i) {
        if (pgm_read_word(&entry->keys[i]) != seq[i]) return false;  // divergent (or entry shorter)
    }
    if (pgm_read_word(&entry->keys[num_seq]) == SUPER_LEADER_END) return true;  // full match
    *partial = true;  // entry continues — a longer layer sequence may still complete
    return false;
}

void super_leader_add_user(const uint16_t* seq, uint8_t num_seq, bool* partial) {
    for (uint8_t i = 0; i < bound_seq_count; ++i) {
        bool entry_partial = false;
        if (ll_entry_matches(&bound_seqs[i], seq, num_seq, &entry_partial)) {
            super_leader_set_match(bound_seqs[i].output);
        } else if (entry_partial) {
            *partial = true;
        }
    }
}

// Leave no trace (same convention as super_leader.def).
#undef LL_KEYS
#undef LL_STR
#undef LL_KEY
#undef LL_UNI
#undef LL_FN
#undef LL_LAYER
