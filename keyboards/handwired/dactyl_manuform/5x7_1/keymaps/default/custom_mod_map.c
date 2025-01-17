#include "leader.h"
#define MAX_MOD_KEYS 8

typedef void (*CallbackFn)(uint16_t keycode, keyrecord_t *record);

typedef struct {
    const uint16_t keycode;
    CallbackFn on_release;
    CallbackFn on_press;
} ModTapCallback;

typedef struct {
    bool is_active;
    bool was_other_key_pressed;
    uint16_t keycode;
} ModState;

ModState mod_states[MAX_MOD_KEYS];
int current_state = 0;

void on_press(uint16_t keycode) {

    mod_states[current_state].is_active = true;
    mod_states[current_state].was_other_key_pressed = false;
    mod_states[current_state].keycode = keycode;

    current_state = (current_state + 1) % MAX_MOD_KEYS;
}

void on_other_press(void) {
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].is_active) {
            mod_states[i].was_other_key_pressed = true;
            register_code(mod_states[i].keycode);
        }
    }
}

bool on_release(uint16_t keycode) {
    int index = -1;
    for (int i = 0; i < MAX_MOD_KEYS; i++) {
        if (mod_states[i].keycode == keycode) {
            index = i;
            break;
        }
    }
    if (index == -1) return false;

    bool key_pressed = mod_states[index].was_other_key_pressed;
    mod_states[index].is_active = false;
    unregister_code(mod_states[index].keycode);

    current_state = (current_state - 1) % MAX_MOD_KEYS;

    return !key_pressed;
}

