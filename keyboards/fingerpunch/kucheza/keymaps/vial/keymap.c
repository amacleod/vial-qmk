#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _GAMING,
    _RGB
};

// +-------+-------+-------+-------+-------+-------+-------+
// |       |       |       |       |       |       |       |
// +-------+-------+-------+-------+-------+-------+-------+
// |       |       |       |       |       |       |       |
// +-------+-------+-------+-------+-------+-------+-------+
// |       |       |       |       |       |       |       |
// +-------+-------+-------+-------+-------+-------+-------+           +-------+
// |       |       |       |       |       |       |                   |       |
// +-------+-------+-------+-------+-------+-------+-------+   +-------+-------+-------+
// |       |       |       |       |       |       |       |   |       |       |       |    <--- 5 way nav
// +-------+-------+-------+       +-------+-------+-------+   +-------+-------+-------+
//                                                                     |       |
//                                                                     +-------+
//                                 +-------+-------+-------+
//                                 |       |       |       |   <--- 3 way thumb
//                                 +-------+-------+-------+

//    ----.
//         '..
//            '
//        +-------+
//        |       |
//        |       |    <---- foot pedal via TRRS
//        |       |
//        +-------+
//
// This keymap is the same as the "default" keymap, but with VIA/Vial
// enabled (see rules.mk / config.h in this directory) so the layout below
// is only the flashed-in starting point -- it's freely remappable at
// runtime from the Vial app.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Gaming layout, shifted one column right of standard WASD placement so
// A/S/D sit under the ring/middle/index fingers in home position:
//
//   Esc  1    2    3    4    5             T
//   P    Tab  Q    W    E    R             G
//   I    Sft  A    S    D    F             B
//   M    Ctl  Z    X    C    V
//   J    K    L         Alt  Fn   Spc
[_GAMING] = LAYOUT_kucheza(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_T,
    KC_P,      KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_G,
    KC_I,      KC_LSFT,   KC_A,      KC_S,      KC_D,      KC_F,      KC_B,
    KC_M,      KC_LCTL,   KC_Z,      KC_X,      KC_C,      KC_V,                           KC_VOLU,
    KC_J,      KC_K,      KC_L,                 KC_LALT,   MO(_RGB),  KC_SPC,     KC_MPRV, KC_MPLY, KC_MNXT,
                                                                                           KC_VOLD,
                                                KC_VOLD,   KC_MUTE,   KC_VOLU,
    KC_LSFT
),

[_RGB] =  LAYOUT_kucheza(
    QK_BOOT,   KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,        _______,
    _______,   RGB_TOG,      RGB_RMOD,     RGB_MOD,      _______,      TG(_GAMING),  _______,
    _______,   RGB_SPI,      RGB_HUI,      RGB_SAI,      RGB_VAI,      KC_END,       _______,
    _______,   RGB_SPD,      RGB_HUD,      RGB_SAD,      RGB_VAD,      _______,                            _______,
    _______,   _______,      _______,                    _______,      _______,      _______,     _______, _______, _______,
                                                                                                           _______,
                                                         _______,      _______,      _______,
    _______
)
};
