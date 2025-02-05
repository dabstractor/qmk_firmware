#include "leader.h"
#define MAX_MOD_KEYS 8

typedef void (*CallbackFn)(uint16_t keycode, keyrecord_t *record);

typedef struct {
    const uint16_t keycode;
    CallbackFn on_release;
    CallbackFn on_press;
} KeyCallback;

typedef struct {
    bool is_active;
    bool was_other_key_pressed;
    bool is_held;
    uint32_t time_activated;
    uint16_t keycode;
} ModState;

ModState mod_states[MAX_MOD_KEYS];

void on_other_press(void) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].is_active) {
            mod_states[i].was_other_key_pressed = true;
            mod_states[i].time_activated = timer_read();
            register_code(mod_states[i].keycode);
        }
    }
}

void on_press(uint16_t keycode) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (!mod_states[i].is_active) {
            mod_states[i].is_active = true;
            mod_states[i].was_other_key_pressed = false;
            mod_states[i].is_held = false;
            mod_states[i].keycode = keycode;
            mod_states[i].time_activated = timer_read();
            break;
        }
    }
}

bool on_release(uint16_t keycode) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].keycode == keycode) {
            bool key_pressed = mod_states[i].was_other_key_pressed;
            mod_states[i].is_active = false;
            unregister_code(mod_states[i].keycode);
            return !key_pressed && !mod_states[i].is_held;
        }
    }
    return false;
}

// runs every clock cycle
void matrix_scan_user(void) {
    if (TAPPING_TERM > 0) {
        for (int i = 0; i < MAX_MOD_KEYS; i++) {
            if (mod_states[i].is_active) {
                if (timer_read() - mod_states[i].time_activated > TAPPING_TERM) {
                    mod_states[i].is_active = false;
                    mod_states[i].is_held = true;
                    register_code(mod_states[i].keycode);
                }
            }
        }
    }
}
