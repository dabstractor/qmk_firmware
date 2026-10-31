# encoder_pairs — layer-pair encoder overlays

Per-application encoder behavior for the utility layers (`_FN`/`_NUMPAD`/`_CMD`/
`_MOUSE`), which otherwise always shadow the QMKonnect app layers in the stock
`encoder_map` transparency walk. The pair table is defined in `keymap.c` next
to `encoder_map`; the dispatcher lives in `encoder_pairs.c` and runs from
`process_record_user` immediately after the powerscroll passthrough.

## Semantics contract (author-facing)

- `[_NEOVIM][_FN]` reads as a possessive: "neovim's FN knob".
- Unwritten pair / `0` cell → stock `encoder_map` behavior. Empty table is
  bit-identical to no feature.
- `_______` (KC_TRNS) in one encoder's slot → that encoder inherits stock even
  inside a written row.
- Any keycode form: basic, modded (`LCTL(KC_TAB)`), keymap macros (`TAB_L`),
  `QK_KB` customs (routed to CUSTOM_KEYS / mod-tap dispatchers — so the whole
  callback system works per-pair for free).
- Blanket + override: `[_QWERTY][_FN]` is inherited by every app layer without
  its own `[_FN]` entry (the context walk probes down the active stack).
- Powerscroll wins: `POWERSCROLL_HOLD` consumes encoder events first; pair
  actions don't get multiplied (revisit if that ever hurts).

## Design notes

- **Fixed-top walk**: `top` is pinned at the highest active layer; only the
  *context* walks down through remaining active layers, probing
  `pair[context][top]` until first hit. Walking `top` down too would wrongly
  fire `[_NEOVIM][_FN]` while `_CMD` is held. Tri-layer keeps `_FN`/`_NUMPAD`
  bits set under `_CMD`, which is exactly why the context walk must skip
  intermediate utility layers on its way to the app layer.
- **Termination is structural**: each loop iteration must clear exactly one
  layer bit (`remaining = rest & ~(1 << context)`). The first implementation
  assigned `remaining = rest`, which never cleared the context bit — an
  infinite loop / hard lock on any encoder roll with ≥2 active layers and an
  empty table. Caught pre-flash during review; do not reintroduce.
- **Direction layout**: `[clockwise ? 0 : 1]` — matches `encoder_map` layout
  (`ENCODER_CCW_CW(ccw, cw)` expands to `{(cw), (ccw)}`).
- **Storage**: flat `uint16_t[32][32][NUM_ENCODERS][2]` = 8 KiB, lands in
  `.rodata` (flash). The `DEFINE_ENCODER_PAIRS` macro is the interface; a
  sparse backend can replace it without touching authoring syntax.

## Gotchas

- `KC_NO` is 0x0000 — indistinguishable from an unwritten cell. A "dead pair
  cell" sentinel constant can be added if ever needed.
- Encoder-map turns arrive as synthetic key events (`row` 253/252 = CW/CCW,
  `col` = encoder index) — that's what `IS_ENCODEREVENT` matches. All dispatch
  runs on the master half; layer state is synced, so both knobs see identical
  context.
- Release halves are re-evaluated (not cached) — if layers change between the
  press and release half of one detent, the release may fall through to stock.
  Harmless for tap-style actions; powerscroll has the same exposure.
- `powerscroll.c`'s `encoder_to_original_keycode()` applies a ±12 offset to
  some keycode ranges that has no counterpart in current QMK source
  (straight table reads). Unresolved mystery — if pair actions ever feed
  powerscroll streams directly, pin this down first.
- Verified against QMK `0.33.9-140` (`layer_switch_get_layer`,
  `keycode_at_encodermap_location`, `IS_ENCODEREVENT` internals). Re-verify
  these if a QMK rebase changes encoder event plumbing.

## Future

- **Key-position overlays**: same walk + fallback rules, sparse-list authored
  (`DEFINE_KEY_PAIRS`) — a flat `[32][32][12][7]` table would be 172 KiB.
  Open question: press-time decision caching vs re-evaluation for release
  symmetry.
- **Module hoisting**: keep `encoder_pairs.c` dependency-free and it can move
  to `modules/` later with the keymap calling `encoder_pairs_process()` as a
  plain function (module chain-ordering can't express "after powerscroll,
  before custom mod-tap", which is why it's keymap-local today).
