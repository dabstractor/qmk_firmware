// leader_layers.c — machinery for the LEADER SEQUENCES section of keymap.c.
// Write once, never touch: every leader definition — global and per-layer —
// lives in keymap.c; this file only turns those definitions into the
// super_leader module's runtime behavior.
//
// How it works (for the curious, not for editing):
//   * The module's own introspected table (super_leader.def) is left EMPTY —
//     the .def file is a permanent stub. Instead, keymap.c defines two
//     sizeof-counted tables via the public L_* macros below, and this
//     file drives matching entirely through the module's documented hook
//     super_leader_add_user(): after each typed key, global entries are
//     matched first, then the bound layer's entries — so a layer entry
//     shadows a global one on an exact key collision, and all resolution
//     semantics (longest match, immediate resolve, replay of divergent
//     keys, timeout, held-key outputs) stay module-native.
//   * When a leader sequence starts, super_leader_start_user() snapshots the
//     app layer (utility layers _NUMPAD/_FN/_CMD/_MOUSE masked out, per the
//     layers.c convention) and binds that layer's block for the sequence's
//     duration — the module ignores layer keys mid-sequence, so "the layer
//     LEADER was pressed on" is the only coherent per-layer definition.
//   * L_FN callbacks ride the LEADER_BRIDGE compile-time trampoline from
//     custom_keycodes.h — any handler by name, no wrappers ever.
#include QMK_KEYBOARD_H
#include "super_leader.h"
#include "custom_keycodes.h"
#include "./layers.c"

// ---- Public entry macros (persistent — used by keymap.c's tables) ----------
// Entries are ordinary comma-separated initializer rows — standard C style,
// trailing comma optional. Inside L_LAYER blocks the same shapes apply; the
// layer is inherited from the enclosing block.
#define L_KEYS1_(...) (const uint16_t[]){ __VA_ARGS__, SUPER_LEADER_END }
#define L_KEYS(keys_) L_KEYS1_ keys_
#define L_STR(keys_, str_) { .keys = L_KEYS(keys_), .output = SUPER_LEADER_STR(str_) }
#define L_KEY(keys_, kc_)  { .keys = L_KEYS(keys_), .output = SUPER_LEADER_KEY(kc_) }
#define L_UNI(keys_, str_) { .keys = L_KEYS(keys_), .output = SUPER_LEADER_UNI(str_) }
#define L_FN(keys_, fn_)   { .keys = L_KEYS(keys_), .output = SUPER_LEADER_FUN(LEADER_BRIDGE(fn_), fn_) }
#define L_PW(...) __VA_ARGS__ SS_TAP(X_ENTER) // password, then Enter

// zk-kb number codegen: single digit 0 = !-10, 1-9 = !-N (:N variant for 1);
// two digits 0X = !-10:X, NX = !-N:X. L_ZK_ALL expands to all 110 entries,
// comma-separated, no trailing comma — treat it like any other entry.
#define L_ZK(...) __VA_ARGS__ SS_TAP(X_TAB) // zk-kb command, then Tab
#define L_ZK_ENTRY_0(elem)     L_STR((KC_0, KC_##elem), L_ZK("!-10:" #elem))
#define L_ZK_ENTRY_N(cmd, elem) L_STR((KC_##cmd, KC_##elem), L_ZK("!-" #cmd ":" #elem))
#define L_ZK_CMD_ELEMS(cmd) \
    L_ZK_ENTRY_N(cmd, 0), L_ZK_ENTRY_N(cmd, 1), L_ZK_ENTRY_N(cmd, 2), L_ZK_ENTRY_N(cmd, 3), L_ZK_ENTRY_N(cmd, 4), \
    L_ZK_ENTRY_N(cmd, 5), L_ZK_ENTRY_N(cmd, 6), L_ZK_ENTRY_N(cmd, 7), L_ZK_ENTRY_N(cmd, 8), L_ZK_ENTRY_N(cmd, 9)
#define L_ZK_ALL \
    L_STR((KC_0), L_ZK("!-10")), L_STR((KC_1), L_ZK("!-1:1")), L_STR((KC_2), L_ZK("!-2")), \
    L_STR((KC_3), L_ZK("!-3")), L_STR((KC_4), L_ZK("!-4")), L_STR((KC_5), L_ZK("!-5")), \
    L_STR((KC_6), L_ZK("!-6")), L_STR((KC_7), L_ZK("!-7")), L_STR((KC_8), L_ZK("!-8")), \
    L_STR((KC_9), L_ZK("!-9")), \
    L_ZK_ENTRY_0(0), L_ZK_ENTRY_0(1), L_ZK_ENTRY_0(2), L_ZK_ENTRY_0(3), L_ZK_ENTRY_0(4), \
    L_ZK_ENTRY_0(5), L_ZK_ENTRY_0(6), L_ZK_ENTRY_0(7), L_ZK_ENTRY_0(8), L_ZK_ENTRY_0(9), \
    L_ZK_CMD_ELEMS(1), L_ZK_CMD_ELEMS(2), L_ZK_CMD_ELEMS(3), L_ZK_CMD_ELEMS(4), \
    L_ZK_CMD_ELEMS(5), L_ZK_CMD_ELEMS(6), L_ZK_CMD_ELEMS(7), L_ZK_CMD_ELEMS(8), \
    L_ZK_CMD_ELEMS(9)

// Table wrappers — entry counts are derived from sizeof, so entries are
// plain comma-separated rows and a trailing comma is genuinely optional.
#define LEADER_GLOBAL(...)                                                \
    const super_leader_sequence_t leader_seqs_global[] = { __VA_ARGS__ }; \
    const uint8_t leader_seqs_global_count =                              \
        sizeof(leader_seqs_global) / sizeof(leader_seqs_global[0]);
#define LEADER_LAYERS(...) \
    const leader_layer_block_t leader_layer_blocks[_MOUSE + 1] = { __VA_ARGS__ };

// Per-layer blocks: designated-init element of the layer-indexed table.
#define L_LAYER(layer_, ...)                                                         \
    [layer_] = { .seqs  = (const super_leader_sequence_t[]){ __VA_ARGS__ },          \
                 .count = sizeof((const super_leader_sequence_t[]){ __VA_ARGS__ }) / \
                          sizeof(super_leader_sequence_t) },

typedef struct {
    const super_leader_sequence_t* seqs;  // NULL = no block for this layer
    uint8_t count;                        // number of entries in seqs
} leader_layer_block_t;

// Defined in keymap.c (LEADER SEQUENCES section) via the wrappers below.
extern const super_leader_sequence_t leader_seqs_global[];
extern const uint8_t leader_seqs_global_count;
extern const leader_layer_block_t leader_layer_blocks[];

#define LEADER_APP_MASK \
    (~(layer_state_t)((1u << _NUMPAD) | (1u << _FN) | (1u << _CMD) | (1u << _MOUSE)))

// The block bound at sequence start.
static const super_leader_sequence_t* bound_seqs;
static uint8_t bound_seq_count;

void super_leader_start_user(void) {
    const leader_layer_block_t* block = &leader_layer_blocks[get_highest_layer(layer_state & LEADER_APP_MASK)];
    bound_seqs      = block->seqs;
    bound_seq_count = block->count;
}

// Module-idiomatic match: does the typed prefix `seq` (length num_seq) fully
// match this entry, or is it a proper prefix of it (keep waiting)?
static bool leader_entry_matches(const super_leader_sequence_t* entry, const uint16_t* seq, uint8_t num_seq, bool* partial) {
    for (uint8_t i = 0; i < num_seq; ++i) {
        if (pgm_read_word(&entry->keys[i]) != seq[i]) return false;  // divergent (or entry shorter)
    }
    if (pgm_read_word(&entry->keys[num_seq]) == SUPER_LEADER_END) return true;  // full match
    *partial = true;  // entry continues — a longer sequence may still complete
    return false;
}

static void leader_match_table(const super_leader_sequence_t* table, uint8_t count, const uint16_t* seq, uint8_t num_seq, bool* partial) {
    for (uint8_t i = 0; i < count; ++i) {
        bool entry_partial = false;
        if (leader_entry_matches(&table[i], seq, num_seq, &entry_partial)) {
            super_leader_set_match(table[i].output);  // last full match wins
        } else if (entry_partial) {
            *partial = true;
        }
    }
}

void super_leader_add_user(const uint16_t* seq, uint8_t num_seq, bool* partial) {
    leader_match_table(leader_seqs_global, leader_seqs_global_count, seq, num_seq, partial);  // first: global defaults
    leader_match_table(bound_seqs, bound_seq_count, seq, num_seq, partial);                   // last: layer shadows global
}
