#include "leader.h"
#define MAX_MOD_KEYS 8

/* #define CUSTOM_SAFE_RANGE (SAFE_RANGE + 12) */
/* #define MOD_MASK 0xFF // 8 bits for modifiers (0–255) */

typedef void (*CallbackFn)(uint16_t keycode, keyrecord_t *record);

typedef struct {
    const uint16_t keycode;
    CallbackFn on_release;
    CallbackFn on_press;
} KeyCallback;

typedef struct {
    const uint16_t keycode;
    const uint16_t mod_code;
} CustomModMap;

typedef struct {
    bool is_active;
    bool was_other_key_pressed;
    bool is_held;
    uint32_t time_activated;
    uint16_t keycode;
    uint16_t mod_code;
} ModState;

ModState mod_states[MAX_MOD_KEYS];


void on_other_press(void) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].is_active) {
            mod_states[i].was_other_key_pressed = true;
            mod_states[i].is_held = true;
            register_code(mod_states[i].mod_code);
        }
    }
}

void on_press(uint16_t keycode, uint16_t mod_code) {
    int found_key = -1;

    // look for existing key
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].keycode == keycode) {
            found_key = i;
            break;
        }
    }

    if (found_key == -1) {
        // look for next available slot
        for (int i = 0; i < MAX_MOD_KEYS; i++) {
            if (!mod_states[i].is_active && !mod_states[i].is_held) {
                found_key = i;
                break;
            }
        }
    }

    // activate any other mod keys that were waiting for another keypress
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].is_active) {
            mod_states[i].was_other_key_pressed = true;
            mod_states[i].is_held = true;
            register_code(mod_states[i].mod_code);
        }
    }

    mod_states[found_key].is_active = true;
    mod_states[found_key].was_other_key_pressed = false;
    mod_states[found_key].is_held = false;
    mod_states[found_key].keycode = keycode;
    mod_states[found_key].mod_code = mod_code;
    mod_states[found_key].time_activated = timer_read();
}

bool on_release(uint16_t keycode, uint16_t mod_code) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].keycode == keycode) {
            bool run_callback = !mod_states[i].was_other_key_pressed && !mod_states[i].is_held;
            mod_states[i].is_active = false;
            mod_states[i].is_held = false;
            mod_states[i].was_other_key_pressed = false;
            unregister_code(mod_states[i].mod_code);
            return run_callback;
        }
    }

    return false;
}

// runs every clock cycle
void matrix_scan_user(void) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        // Only act if the key is still active AND it has not already been triggered
        int tapping_term = get_tapping_term(mod_states[i].keycode, NULL);
        if (mod_states[i].is_active && timer_read() - mod_states[i].time_activated > tapping_term) {
            mod_states[i].is_held = true;
            register_code(mod_states[i].mod_code);
        }
    }
}


