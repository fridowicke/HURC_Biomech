#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "basic_LCD.pio.h"
#include "LCD_interface.h"

/* This file implements the low-level interface with the LCD, along with some graphics primitives
optimized using low-level commands. Higher-level graphics are handled in graphics.c and menu.c.
*/

PIO pio;
uint sm;

void LCD_dat(uint8_t byte) {
    pio_sm_put_blocking(pio, sm, byte); // Write data
}

void LCD_cmd(uint8_t byte) {
    sleep_us(1);
    gpio_put(LCD_CD, false);
    pio_sm_put_blocking(pio, sm, byte); // Write data
    sleep_us(1);
    gpio_put(LCD_CD, true);
}

void LCD_init() {
    // Control Pins
    gpio_init(LCD_RD);
    gpio_init(LCD_CD);
    gpio_init(LCD_WR);
    gpio_init(LCD_CS);
    gpio_init(LCD_RST);

    gpio_set_dir(LCD_RD, true);
    gpio_set_dir(LCD_CD, true);
    gpio_set_dir(LCD_WR, true);
    gpio_set_dir(LCD_CS, true);
    gpio_set_dir(LCD_RST, true);

    gpio_put(LCD_RST, true);
    gpio_put(LCD_RD, true);
    gpio_put(LCD_CD, true);
    gpio_put(LCD_WR, true);
    gpio_put(LCD_CS, true);

    pio = pio0;

    uint offset = pio_add_program(pio, &basic_LCD_program);

    sm = pio_claim_unused_sm(pio, true);
    basic_LCD_program_init(pio, sm, offset, LCD_PARALLEL_BASE, LCD_WR, 4); // Base LCD parallel interface pin

    // Power on LCD after waiting a bit
    sleep_ms(5);

    gpio_put(LCD_CS, false); // Select LCD
    LCD_cmd(0x01); // Software reset
    sleep_us(50);
    LCD_cmd(0x28); // Display off
    LCD_cmd(0x11); // Sleep out
    sleep_us(150);
    LCD_cmd(0x29); // LCD_ON
    sleep_us(500);
    LCD_cmd(0x3A); // COLMOD
    LCD_dat(0b00000110);
    LCD_cmd(0x36); // MADCTL
    LCD_dat(0b00101000);
}

void LCD_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    LCD_cmd(0x2A); // CASET
    LCD_dat(x >> 8);
    LCD_dat(x);
    LCD_dat((x + width - 1) >> 8);
    LCD_dat(x + width - 1);

    LCD_cmd(0x2B); // PASET
    LCD_dat(y >> 8);
    LCD_dat(y);
    LCD_dat((y + height - 1) >> 8);
    LCD_dat(y + height - 1);
}

// Flood screen with color c. Note top 8 bits of color aren't used. Currently equivalent to rect
void LCD_flood(uint32_t color) {
    // Window and send write data command
    LCD_window(0, 0, 320, 240);

    LCD_cmd(0x2c); // RAMWR

    for(uint16_t x = 0; x < 320; x++) { // 3 bytes per pixel, 240x320 display
        for(uint16_t y = 0; y < 240; y++) {
            LCD_dat((color >> 16) & 0xFF); // Red
            LCD_dat((color >> 8) & 0xFF); // Green
            LCD_dat(color & 0xFF); // Blue
        }
    }
}

void LCD_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    // Send write data command
    LCD_window(x, y, width, height);

    LCD_cmd(0x2C); // RAMWR

    for(uint32_t pix = 0; pix < width * height; pix++) { // 3 bytes per pixel
        LCD_dat((color >> 16) & 0xFF); // Red
        LCD_dat((color >> 8) & 0xFF); // Green
        LCD_dat(color & 0xFF); // Blue
    }
}

void LCD_buffer(uint8_t *buf, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    // Set window and clock out graphics buffer
    LCD_window(x, y, width, height);

    LCD_cmd(0x2C); // RAMWR

    for(uint32_t pix = 0; pix < width * height; pix++) {
        LCD_dat(buf[pix * 3]); // Red
        LCD_dat(buf[pix * 3 + 1]); // Green
        LCD_dat(buf[pix * 3 + 2]); // Blue
    }
}