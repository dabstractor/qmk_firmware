# VIA runtime keymap editor (upstream QMK — NOT the vial-qmk fork).
# Force-enables raw_hid + dynamic_keymap + bootmagic + tri_layer, all of which
# the keyboard already has on; enabling here is a no-op for those four.
VIA_ENABLE = yes

# The matching DYNAMIC_KEYMAP_LAYER_COUNT (32) and the enlarged wear-leveling
# EEPROM backing live elsewhere by necessity:
#
#   * layer_count: set in keyboard.json (dynamic_keymap.layer_count), because
#     that is the ONE location the data-driven config chain honors — config.h
#     #define and -D / OPT_DEFS are both silently shadowed for this key. 32, not
#     the 14 you'd count by eye, because layers.c uses sparse high-priority
#     indices (_NUMPAD=28, _FN=29, _CMD=30, _MOUSE=31) via designated
#     initializers, so the compiled keymaps[] array is 32 slots.
#
#   * WEAR_LEVELING_BACKING_SIZE (16384 -> 8 KiB logical): in this keymap's
#     config.h, because 32 layers × 12 × 7 × 2 = 5376 bytes of dynamic keymap
#     alone exceeds the RP2040 default 4 KiB logical EEPROM.