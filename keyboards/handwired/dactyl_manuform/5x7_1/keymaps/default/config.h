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

#define MASTER_RIGHT
// #define DEBOUNCE 5

#define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode
// // #define SERIAL_USART_HALF_DUPLEX   // Enable half duplex operation mode
#define SERIAL_USART_RX_PIN GP0
#define SERIAL_USART_TX_PIN GP1
// #define SERIAL_USART_TX_PIN GP0

#define SPLIT_HAND_PIN GP8 // Shunt from 3v to GP26 on left side
// #define	SPLIT_HAND_PIN_LOW_IS_LEFT

// #define USE_SERIAL

// #define SOFT_SERIAL_PIN D0

// #define SPLIT_USB_DETECT
// #define SOFT_SERIAL_PIN GP0

