/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define EE_HANDS

#define SERIAL_USART_FULL_DUPLEX // Enable full duplex operation mode.
#define SERIAL_USART_TX_PIN GP0  // USART TX pin
#define SERIAL_USART_RX_PIN GP1  // USART RX pin

#define ENCODER_DEFAULT_POS 0x3
#define ENCODER_DEBOUNCE 5

#define LAYER_STATE_32BIT

#define TAPPING_TERM_PER_KEY

#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY

#define LEADER_NO_TIMEOUT
#define LEADER_PER_KEY_TIMING

#define RETRO_TAPPING
#define RETRO_TAPPING_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_F18

#define MK_COMBINED
#define MOUSEKEY_DELAY 1
#define MOUSEKEY_MOVE_DELTA 4
#define MOUSEKEY_MAX_SPEED 20
#define MOUSEKEY_TIME_TO_MAX 50

#define BETTER_VIM_MODE
#define VIM_G_MOTIONS
#define VIM_PASTE_BEFORE
#define VIM_REPLACE
#define VIM_DOT_REPEAT
#define VIM_W_BEGINNING_OF_WORD
#define VIM_NUMBERED_JUMPS
#define VIM_FOR_ALL
#define VIM_ESC_PASSTHROUGH

#define POWERSCROLL_LEVEL 10 // over 9000
#define POWERSCROLL_DELAY_MS 9 // fine-tuned for terminal input

#define IS_COMMAND() (get_mods() == MOD_MASK_ALT)

// VIA runtime keymap editor
// ---------------------------------------------------------------------------
// 32 layers because the keymap uses sparse, high-priority indices
// (_NUMPAD=28, _FN=29, _CMD=30, _MOUSE=31) via designated initializers, so the
// compiled keymaps[] array is 32 slots and DYNAMIC_KEYMAP_LAYER_COUNT must match.
// 32 layers * 12 rows * 7 cols * 2 bytes = 5376 bytes of dynamic-keymap EEPROM,
// which exceeds the RP2040 default wear-leveling logical size of 4096 — hence
// the enlarged backing below. The count itself is set in keyboard.json
// (dynamic_keymap.layer_count) because that's the one location the data-driven
// config chain honors for this key.
#define WEAR_LEVELING_BACKING_SIZE 16384   // 16 KiB flash -> 8 KiB logical EEPROM
