#pragma once

#include QMK_KEYBOARD_H

// Powerscroll function declarations
bool powerscroll_process(uint16_t keycode, keyrecord_t *record);
bool handle_encoder_update(uint8_t index, bool clockwise);
void powerscroll_matrix_scan(void);