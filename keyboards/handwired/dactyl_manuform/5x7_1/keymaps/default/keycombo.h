#ifndef COMBO_H
#define COMBO_H

/* --- Implementation Functions (private) --- */
// These are the real, optimized functions. Making them static inline
// allows the compiler to eliminate the function call overhead entirely.

// Handles 1 modifier + key
static inline void combo_impl_2(uint16_t mod1, uint16_t key) {
    register_code(mod1);
    register_code(key);
    unregister_code(key);
    unregister_code(mod1);
}

// Handles 2 modifiers + key
static inline void combo_impl_3(uint16_t mod1, uint16_t mod2, uint16_t key) {
    register_code(mod1);
    register_code(mod2);
    register_code(key);
    unregister_code(key);
    unregister_code(mod2);
    unregister_code(mod1);
}

// Handles 3 modifiers + key
static inline void combo_impl_4(uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t key) {
    register_code(mod1);
    register_code(mod2);
    register_code(mod3);
    register_code(key);
    unregister_code(key);
    unregister_code(mod3);
    unregister_code(mod2);
    unregister_code(mod1);
}

// Handles 4 modifiers + key
static inline void combo_impl_5(uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, uint16_t key) {
    register_code(mod1);
    register_code(mod2);
    register_code(mod3);
    register_code(mod4);
    register_code(key);
    unregister_code(key);
    unregister_code(mod4);
    unregister_code(mod3);
    unregister_code(mod2);
    unregister_code(mod1);
}

/* --- Macro Magic for Developer Experience --- */
// This macro counts the arguments and calls the correct implementation.
// The last argument passed to the macro becomes the 'NAME' of the function to call.
#define GET_COMBO_IMPL(_1, _2, _3, _4, _5, NAME, ...) NAME

/**
 * @brief Sends a key combination with 1 to 4 modifiers.
 *
 * Usage: combo(MOD_1, MOD_2, ..., KEY)
 * Example: combo(KC_LSHIFT, KC_A)
 * Example: combo(KC_LCTL, KC_LALT, KC_DEL)
 */
#define combo(...) GET_COMBO_IMPL(__VA_ARGS__, combo_impl_5, combo_impl_4, combo_impl_3, combo_impl_2)(__VA_ARGS__)

#endif // COMBO_H
