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

#define MASTER_LEFT
//#define MASTER_RIGHT

//#define EE_HANDS

#define SPLIT_HAND_PIN GP1

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17


#define HAL_USE_SERIAL TRUE
#define SERIAL_USART_TX_PIN GP1     // The GPIO pin that is used split communication.

#define HAL_USE_SIO TRUE

#define USE_SERIAL
#define SERIAL_USART_HALF_DUPLEX
#define SERIAL_USART_TX_PIN GP1

