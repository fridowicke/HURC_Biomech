#include "graphics.h"
#include "LCD_interface.h"
#include "icons.h"
#include "actuator_interface.h"
#include "actuator_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/malloc.h"
#include "bitmapFont.h"
#include "menu_defs.h"

void graphics_text(uint16_t x, uint16_t y, char *string, float size, uint32_t color, uint32_t backgroundColor) {
    uint8_t len = strlen(string);

    uint16_t buf_len = len * 8 * 8; // 8 pixel by 8*len array of 3 8-bit subpixels
    uint8_t *buf = malloc(sizeof(uint8_t) * buf_len * 3);

    // Fill graphics buffer
    for(uint8_t i = 0; i < len; i++) {
        uint8_t asciiCode = string[i] - 32;

        // Iterate through character array
        for(uint8_t ix = 0; ix < 8; ix++) {
            for(uint8_t iy = 0; iy < 8; iy++) {
                uint16_t buf_index = (iy * len * 8) + (ix + 8 * i); // y * y length plus x

                if(bitmap_font[asciiCode][iy] & (1 << ix)) {
                    buf[buf_index * 3] = (color >> 16) & 0xFF;
                    buf[buf_index * 3 + 1] = (color >> 8) & 0xFF;
                    buf[buf_index * 3 + 2] = color & 0xFF;
                } else {
                    buf[buf_index * 3] = (backgroundColor >> 16) & 0xFF;
                    buf[buf_index * 3 + 1] = (backgroundColor >> 8) & 0xFF;
                    buf[buf_index * 3 + 2] = backgroundColor & 0xFF;
                }
            }
        }
    }

    // Scale, display graphics buffer, then free

    buf = graphics_scale(buf, len * 8, 8, size, size);

    LCD_buffer(buf, x, y, len * 8 * size, 8 * size);

    free(buf);
}

// Frees old buffer, replaces it with new one scaled by factors

uint8_t* graphics_scale(uint8_t* buf, uint16_t width, uint16_t height, float xfactor, float yfactor) {
    uint8_t* retbuf = malloc(width * height * xfactor * yfactor * 3);

    uint16_t new_width = width * xfactor;
    uint16_t new_height = height * yfactor;

    for(uint16_t col = 0; col < new_width; col++) {
        for(uint16_t row = 0; row < new_height; row++) {
            // Get pixel color values from closest pixel in start buffer

            uint16_t src_x = round(col / xfactor);
            uint16_t src_y = round(row / yfactor);

            // Ensure new x/y values are within range
            if(src_x >= width) {
                src_x = width-1;
            }
            if(src_y >= height) {
                src_y = height-1;
            }

            uint32_t buf_index = src_y * width + src_x;

            // Repeat that value into the return buffer
            uint32_t retbuf_index = new_width * row + col;

            for(uint8_t i = 0; i < 3; i++) {
                retbuf[retbuf_index * 3 + i] = buf[buf_index * 3 + i];
            }
        }
    }

    free(buf);

    return retbuf;
}

void graphics_value(uint16_t x, uint16_t y, valueID id, float size, uint32_t color, uint32_t backgroundColor) {
    uint32_t value = 0;

    // Call appropriate sensor function based on id
    switch(id) {
        case ACTUATOR_POSITION:
            value = actuator_get_position();
            break;
        case ACTUATOR_FORCE:
            value = actuator_get_force();
            break;
        case ACTUATOR_SETPOINT:
            value = motorControl_getTarget();
            break;
    }

    char displayString[10];

    sprintf(displayString, "%lu", value);

    graphics_text(x, y, displayString, size, color, backgroundColor);
}

void graphics_setting(uint16_t x, uint16_t y, settings setting, float size, uint32_t color, uint32_t backgroundColor) {
    uint16_t value = flash_get_setting(setting);

    char displayString[5];

    sprintf(displayString, "%lu", value);

    // Display setting value
    graphics_text(x, y, displayString, size, color, backgroundColor);
}

void graphics_icon(uint16_t x, uint16_t y, iconType icon, float size) {
    uint8_t* buf = malloc(sizeof(uint8_t) * 400 * 3);

    unsigned char* icon_src;

    // Get icon data from icon header
    switch(icon) {
        case SETTINGS_GEAR:
            icon_src = settings_gear;
            break;
        case BATTERY_WIDGET:
            icon_src = battery_widget;
            break;
        case BACK_ARROW:
            icon_src = settings_gear;
            break;
    }

    // Translate 1-bit buffer to 3-byte pixel image format
    for(uint16_t i = 0; i < 400; i++) {
        if(icon_src[i] != 0) {
            for(uint8_t di = 0; di < 3; di++) {
                buf[i * 3 + di] = 0xFF;
            }
        } else {
            for(uint8_t di = 0; di < 3; di++) {
                buf[i * 3 + di] = 0x00;
            }
        }
    }

    // Display and free the buffer
    buf = graphics_scale(buf, 20, 20, size, size);

    LCD_buffer(buf, x, y, 20 * size, 20 * size);

    free(buf);
}