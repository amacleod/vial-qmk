#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _GAMING,
    _RGB
};

enum custom_keycodes {
    // Snaps RGB back to the intended default (Gradient Up-Down, standard
    // hue/sat/val) and persists it, in case the saved mode ever ends up
    // stale/wrong after a future firmware change shifts effect indices
    // around (has already happened a couple of times during development).
    RGB_RESET = SAFE_RANGE,
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
    _______,   RGB_TOG,      RGB_RMOD,     RGB_MOD,      RGB_RESET,    TG(_GAMING),  _______,
    _______,   RGB_SPI,      RGB_HUI,      RGB_SAI,      RGB_VAI,      KC_END,       _______,
    _______,   RGB_SPD,      RGB_HUD,      RGB_SAD,      RGB_VAD,      _______,                            _______,
    _______,   _______,      _______,                    _______,      _______,      _______,     _______, _______, _______,
                                                                                                           _______,
                                                         _______,      _______,      _______,
    _______
)
};

#ifdef RGB_MATRIX_ENABLE
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_RESET:
            if (record->event.pressed) {
                rgb_matrix_mode_noeeprom(RGB_MATRIX_GRADIENT_UP_DOWN);
                rgb_matrix_sethsv_noeeprom(0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                fp_rgb_set_config_from_current_values();
            }
            return false;
    }
    return true;
}
#endif

#ifdef RGB_MATRIX_ENABLE
// The 9 "accent light" LEDs (indices 33-41 in kucheza.c's g_led_config)
// aren't under any key. Most enabled modes (gradient/breathing/solid)
// already paint every LED uniformly, so they match automatically.
//
// This hook runs after the core effect has already rendered the frame, so
// it can selectively repaint on top of it -- used here for two things:
//
//  - BREATHING: repaint the *whole* board at 0.5x the core effect's
//    speed (0.75x still felt too fast on hardware). Still tracks
//    RGB_SPD/RGB_SPI proportionally like every other mode, just scaled
//    down -- not a fixed rate.
//  - SPLASH: the accent LEDs are far from any key, so Splash's
//    keypress-distance ripple rarely reaches them and they'd otherwise
//    sit idle. Give them a rainbow spread across all 9 (full hue range,
//    left = low hue) that scrolls leftward over time -- each LED's own
//    hue still advances at the ~20s/rotation base rate, but since a full
//    left-shift by one LED only takes 1/9th of that (the LEDs are spaced
//    a ninth of the hue wheel apart), the scrolling motion reads as
//    noticeably faster than a single LED's own color cycling would.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch (rgb_matrix_get_mode()) {
        case RGB_MATRIX_BREATHING: {
            HSV      hsv        = rgb_matrix_config.hsv;
            uint8_t  slow_speed = (uint8_t)(((uint16_t)rgb_matrix_config.speed) / 2);
            uint16_t time       = scale16by8(g_rgb_timer, slow_speed / 8);
            hsv.v               = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
            RGB rgb              = hsv_to_rgb(hsv);
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        }
        case RGB_MATRIX_SPLASH: {
            const uint8_t accent_first = 33;
            const uint8_t accent_count = 9;
            const uint8_t hue_step     = 256 / accent_count;
            uint8_t       base_hue     = (uint8_t)(g_rgb_timer / 80); // ~20s per full rotation
            for (uint8_t idx = 0; idx < accent_count; idx++) {
                uint8_t i = accent_first + idx;
                if (i >= led_min && i < led_max) {
                    HSV hsv = rgb_matrix_config.hsv;
                    hsv.h   = (uint8_t)(base_hue + idx * hue_step);
                    hsv.s   = 255;
                    RGB rgb = hsv_to_rgb(hsv);
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
