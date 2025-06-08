/**
 * @brief  From a general modifier mask (e.g., MOD_MASK_ALT), determines which
 *         specific L/R modifiers are actually being held down.
 * @param  general_mask The general mask to investigate (MOD_MASK_ALT, etc.).
 * @return A bitmask of the specific L/R modifiers that are currently active.
 */
uint8_t get_specific_mods_from_mask(uint8_t general_mask) {
    uint8_t current_mods = get_mods();
    uint8_t specific_mods = 0;

    if (general_mask & MOD_MASK_ALT) {
        if (current_mods & MOD_BIT(KC_LALT)) { specific_mods |= MOD_BIT(KC_LALT); }
        if (current_mods & MOD_BIT(KC_RALT)) { specific_mods |= MOD_BIT(KC_RALT); }
    }
    if (general_mask & MOD_MASK_CTRL) {
        if (current_mods & MOD_BIT(KC_LCTL)) { specific_mods |= MOD_BIT(KC_LCTL); }
        if (current_mods & MOD_BIT(KC_RCTL)) { specific_mods |= MOD_BIT(KC_RCTL); }
    }
    if (general_mask & MOD_MASK_SHIFT) {
        if (current_mods & MOD_BIT(KC_LSFT)) { specific_mods |= MOD_BIT(KC_LSFT); }
        if (current_mods & MOD_BIT(KC_RSFT)) { specific_mods |= MOD_BIT(KC_RSFT); }
    }
    if (general_mask & MOD_MASK_GUI) {
        if (current_mods & MOD_BIT(KC_LGUI)) { specific_mods |= MOD_BIT(KC_LGUI); }
        if (current_mods & MOD_BIT(KC_RGUI)) { specific_mods |= MOD_BIT(KC_RGUI); }
    }
    return specific_mods;
}

/**
 * @brief Checks a modifier mask; if active, it suspends the specific L/R mods,
 *        runs the true_block, and restores them. Otherwise, it runs the false_block.
 * @param mod_mask The general modifier mask to check against (e.g., MOD_MASK_ALT).
 * @param true_block The code to execute if the modifier is active.
 * @param false_block The code to execute if the modifier is not active.
 */
#define IF_MODS_ELSE(mod_mask, true_block, false_block) \
    do { \
        if (get_mods() & (mod_mask)) { \
            uint8_t suspended_mods = get_specific_mods_from_mask(mod_mask); \
            if (suspended_mods) { unregister_mods(suspended_mods); } \
            { true_block; } \
            if (suspended_mods) { register_mods(suspended_mods); } \
        } else { \
            { false_block; } \
        } \
    } while (0)
