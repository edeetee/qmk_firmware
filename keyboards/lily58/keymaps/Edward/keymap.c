#include QMK_KEYBOARD_H

enum layer_number {
    _QWERTY = 0,
    _EXTRAS = 1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
    {[0] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL, KC_TAB, LCTL_T(KC_A), LALT_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F), KC_G, KC_H, LSFT_T(KC_J), LGUI_T(KC_K), LALT_T(KC_L), LCTL_T(KC_SCLN), KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC, KC_RBRC, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT, LSFT_T(KC_CAPS), KC_LCTL, KC_LALT, LGUI_T(KC_SPC), LSFT_T(KC_SPC), MO(1), KC_MINS, KC_EQL), [1] = LAYOUT(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_TRNS, KC_TRNS, KC_1, KC_2, KC_3, KC_TRNS, KC_END, KC_PGDN, KC_PGUP, KC_HOME, KC_TRNS, KC_F12, KC_TRNS, KC_TRNS, KC_4, KC_5, KC_6, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_7, KC_8, KC_9, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)};

layer_state_t layer_state_set_user(layer_state_t state) {
    return state;
    // return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

// SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // if (!is_keyboard_master()) return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    return OLED_ROTATION_270;
}

#    include "quantum.h"
#    include <stdio.h>

char layer_state_vert_str[5];

const char *read_layer_state_vert(void) {
    switch (layer_state) {
        case _QWERTY:
            snprintf(layer_state_vert_str, sizeof(layer_state_vert_str), "QWERT");
            break;
        case _EXTRAS:
            snprintf(layer_state_vert_str, sizeof(layer_state_vert_str), "FN");
            break;
        default:
            snprintf(layer_state_vert_str, sizeof(layer_state_vert_str), "%u", layer_state);
    }

    return layer_state_vert_str;
}

// When you add source files to SRC in rules.mk, you can use functions.
// const char *read_layer_state(void);
const char *read_logo(void);
void        set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

const char *read_mode_icon(bool swap);
const char *read_host_led_state(void);
void        set_timelog(void);
const char *read_timelog(void);

#    define KEYLOG_LINES 3
char keylog_str_vert[KEYLOG_LINES][5] = {};

const char code_to_name[60];

void set_keylog_vert(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(keylog_str_vert[0], sizeof(keylog_str_vert[0]), "%dx%d", record->event.key.row, record->event.key.col);
    snprintf(keylog_str_vert[1], sizeof(keylog_str_vert[1]), "k%2d", keycode);
    snprintf(keylog_str_vert[2], sizeof(keylog_str_vert[2]), "%c", name);
}

void write_keylog_vert(void) {
    for (int i = 0; i < KEYLOG_LINES; i++) {
        oled_write_ln(keylog_str_vert[i], false);
        oled_advance_page(true);
    }
}

void draw_stripe(int size, int thickness, bool flip, bool color) {
    for (int x = 0; x < size; x++) {
        for (int y = x - thickness; y < x + thickness; y++) {
            if (rand() % 2 == 0) {
                continue;
            }
            if (flip) {
                oled_write_pixel(size - x - 1, y, color);
            } else {
                oled_write_pixel(x, y, color);
            }
        }
    }
}

bool stripe(int x, int y, int thickness) {
    return abs(x - y) < thickness;
}

bool animated_hash(int x, int y);

void oled_write_layer_state(void) {
    int random_pixels = 10;
    // oled_clear();
    for (int i = 0; i < random_pixels; i++) {
        int x = rand() % 32;
        int y = rand() % 64;

        bool color = false;

        switch (layer_state) {
            case 0:
                color |= animated_hash(x, y);
                break;
            case 2:
                color |= stripe(x, y, 2);
                color |= stripe(32 - x, y, 2);
                break;
        }

        // color &= rand() < RAND_MAX * 0.5;

        oled_write_pixel(x, y, color);
    }

    oled_set_cursor(0, 9);

    snprintf(layer_state_vert_str, sizeof(layer_state_vert_str), "%u", layer_state);
    oled_write_ln(layer_state_vert_str, false);
    oled_advance_page(true);
}

#    define Y_MAX 128

void write_anim_logo(void) {
    int random_pixels = 20;
    // oled_pan(false);
    for (int i = 0; i < random_pixels; i++) {
        int  x     = rand() % 32;
        int  y     = rand() % Y_MAX;
        bool color = animated_hash(x, y) && rand() < RAND_MAX * 0.9;

        oled_write_pixel(x, y, color);
    }
}

bool animated_hash(int x, int y) {
    int modulus = 7;
    int offset  = (timer_read() / 30000);
    // y += offset;

    int blank_offset = 2;

    if ((y / modulus + offset) % blank_offset == 0 || (x / modulus + offset) % blank_offset == 0) return false;

    bool stripe_color = stripe(modulus - x % modulus, y % modulus, 1) | stripe(x % modulus, y % modulus, 1);

    return stripe_color;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_write_layer_state();
        write_keylog_vert();
    } else {
        write_anim_logo();
    }
    return false;
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
#ifdef OLED_ENABLE
        set_keylog_vert(keycode, record);
#endif
        // set_timelog();
    }
    return true;
}
