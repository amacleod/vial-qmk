#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

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
    QK_BOOT,   KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,        AU_TOGG,
    _______,   RGB_TOG,      RGB_RMOD,     RGB_MOD,      _______,      TG(_GAMING),  _______,
    _______,   RGB_SPI,      RGB_HUI,      RGB_SAI,      RGB_VAI,      KC_END,       _______,
    _______,   RGB_SPD,      RGB_HUD,      RGB_SAD,      RGB_VAD,      _______,                            _______,
    _______,   _______,      _______,                    _______,      _______,      _______,     _______, _______, _______,
                                                                                                           _______,
                                                         _______,      _______,      _______,
    _______
)
};

#ifdef RGB_MATRIX_ENABLE
// The 9 "accent light" LEDs (indices 33-41 in kucheza.c's g_led_config)
// aren't under any key. Most enabled modes (gradient/breathing/solid)
// already paint every LED uniformly, so they match automatically.
//
// This hook runs after the core effect has already rendered the frame, so
// it can selectively repaint on top of it -- used here for two things:
//
//  - BREATHING: repaint the *whole* board at 0.75x the core effect's
//    speed (observed ~40 breaths/min at the current speed setting; this
//    targets ~30/min). The core BREATHING effect's own speed always
//    tracks the shared RGB_SPD/RGB_SPI setting (used by other modes too),
//    so slowing just this one down means recomputing it entirely rather
//    than adjusting a shared setting.
//  - SPLASH: the accent LEDs are far from any key, so Splash's
//    keypress-distance ripple rarely reaches them and they'd otherwise
//    sit idle. Give them their own slow, ambient rainbow cycle instead
//    (~20s per full hue rotation), independent of key activity.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch (rgb_matrix_get_mode()) {
        case RGB_MATRIX_BREATHING: {
            HSV      hsv        = rgb_matrix_config.hsv;
            uint8_t  slow_speed = (uint8_t)(((uint16_t)rgb_matrix_config.speed * 3) / 4);
            uint16_t time       = scale16by8(g_rgb_timer, slow_speed / 8);
            hsv.v               = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
            RGB rgb              = hsv_to_rgb(hsv);
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }
        case RGB_MATRIX_SPLASH: {
            HSV hsv = rgb_matrix_config.hsv;
            hsv.h   = (uint8_t)(g_rgb_timer / 80); // ~20s per full rotation
            hsv.s   = 255;
            RGB rgb = hsv_to_rgb(hsv);
            for (uint8_t i = 33; i <= 41; i++) {
                if (i >= led_min && i < led_max) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
            break;
        }
        default:
            break;
    }
    return true;
}
#endif
