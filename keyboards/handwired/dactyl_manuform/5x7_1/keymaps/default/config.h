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

#define IS_COMMAND() (get_mods() == MOD_MASK_ALT)
