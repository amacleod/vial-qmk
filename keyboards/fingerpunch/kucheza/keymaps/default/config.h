/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once
#define VIAL_KEYBOARD_UID {0xA4, 0x0C, 0x5A, 0x2B, 0x05, 0xDF, 0xD9, 0xB9}

#if defined(ENCODER_ENABLE) && defined(VIAL_ENCODERS_ENABLE)
#define VIAL_ENCODER_DEFAULT { KC_VOLD, KC_VOLU, \
                               KC_TRNS, KC_TRNS, \
                               KC_TRNS, KC_TRNS, \
                               KC_TRNS, KC_TRNS }
#endif

#ifndef VIAL_INSECURE
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }
#endif

// For via/vial only
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
