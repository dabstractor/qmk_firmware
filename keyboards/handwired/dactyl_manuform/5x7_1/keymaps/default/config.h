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
// #define MASTER_LEFT
// #define DEBOUNCE 5

// #define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode
// #define SERIAL_USART_RX_PIN D2
// #define SERIAL_USART_TX_PIN D3
// // #define SERIAL_USART_HALF_DUPLEX   // Enable half duplex operation mode
#define SERIAL_USART_RX_PIN GP0
#define SERIAL_USART_TX_PIN GP1
// #define SERIAL_USART_TX_PIN GP0

// #define SPLIT_HAND_PIN F4 // No shunt here, settings different for other hand
// #define SPLIT_HAND_PIN_LOW_IS_LEFT True

#define USE_SERIAL

#define SOFT_SERIAL_PIN D2

#define SPLIT_USB_DETECT

// #define ENCODERS_PAD_A { F4 }
// #define ENCODERS_PAD_B { D1 }
// #define ENCODER_RESOLUTION 4

// Define if your encoder has a button
#define ENCODER_BUTTON_PIN D0

// #define SPLIT_HAND_MATRIX_GRID D1, F4
// #define SPLIT_HAND_PIN F4
// #define	SPLIT_HAND_PIN_LOW_IS_LEFT
// #define SOFT_SERIAL_PIN GP0

// #define SPLIT_TRANSPORT_MIRROR

// #define ENCODERS_PAD_A { GP27 }
// #define ENCODERS_PAD_B { GP28 }

// #define MATRIX_COL_PINS_LEFT { F5, F6, F7, B1, B3, B2, B6 }
// #define MATRIX_COL_PINS_RIGHT { F5, F6, F7, B1, B3, B2, B6 }
// #define MATRIX_COL_PINS_RIGHT { B6, B2, B3, B1, F7, F6, F5 }
