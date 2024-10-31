#include QMK_KEYBOARD_H

enum layer_number {
    _COLEMAK = 0,
    _QWERTY  = 1,
    _EXTRAS  = 2,
};

// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS];

#include "jsonmap.c"

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

#    define LEN_VERT_COMBO 4
char layer_state_vert_combo_str[LEN_VERT_COMBO];

const char *layer_state_vert_combo(void) {
    layer_state_vert_combo_str[0] = layer_state_is(_COLEMAK) || true ? 'C' : ' ';
    layer_state_vert_combo_str[1] = layer_state_is(_QWERTY) ? 'Q' : ' ';
    layer_state_vert_combo_str[2] = layer_state_is(_EXTRAS) ? 'X' : ' ';
    layer_state_vert_combo_str[3] = is_caps_word_on() ? 'S' : ' ';

    return layer_state_vert_combo_str;
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

uint8_t oled_rotation_width;

void write_modulo(int chars, const char *text, int strlen) {
    // int chars = lines * oled_rotation_width;

    for (int x = 0; x < chars; x++) {
        int line = x / oled_rotation_width / OLED_FONT_WIDTH;
        int i    = (line % 2 == 0 ? x : oled_rotation_width - (x % oled_rotation_width)) % strlen;
        oled_write_char(text[i], false);
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
    // int random_pixels = 10;
    // oled_clear();
    // int layer = get_highest_layer(layer_state);
    // for (int i = 0; i < random_pixels; i++) {
    //     int x = rand() % 32;
    //     int y = rand() % 64;

    //     bool color = false;

    //     switch (layer) {
    //         case _COLEMAK:
    //             color |= animated_hash(x, y);
    //             break;
    //         case _QWERTY:
    //             color |= stripe(x, y, 1);
    //             color |= stripe(8 - x, y, 2);
    //             break;
    //         case _EXTRAS:
    //             color |= stripe(x, y, 5);
    //             color |= stripe(32 - x, y, 2);
    //             break;
    //     }

    //     // color &= rand() < RAND_MAX * 0.5;

    //     oled_write_pixel(x, y, color);
    // }

    // oled_set_cursor(0, 0);
    write_modulo(LEN_VERT_COMBO * 12, layer_state_vert_combo(), LEN_VERT_COMBO);

    // oled_set_cursor(0, 9)trarstrfwfwqwffpwqwfpazxcdneioly;luy;jluy;

    // oled_write_ln(layer_state_vert_combo(), false);

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
