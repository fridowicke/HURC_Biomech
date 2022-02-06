#include "touch_interface.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/address_mapped.h"

int touch_getX() {
    // Set up pins. XL and XR driving, YU input, YD tristated + pulldown
    gpio_set_dir(touch_XL, true);
    gpio_set_drive_strength(touch_XL, GPIO_DRIVE_STRENGTH_12MA);

    gpio_init(touch_XR);
    gpio_set_dir(touch_XR, true);
    gpio_set_drive_strength(touch_XR, GPIO_DRIVE_STRENGTH_12MA);

    gpio_set_dir(touch_YD, false);
    gpio_pull_down(touch_YD);

    adc_gpio_init(touch_YU);
    adc_select_input(touch_XIN);

    // Get reading 1
    gpio_put(touch_XL, false);
    gpio_put(touch_XR, true);

    sleep_us(touch_sample_delay);
    uint16_t x1 = adc_read();

    // Get reading 2 with reverse polarity
    gpio_put(touch_XL, true);
    gpio_put(touch_XR, false);

    sleep_us(touch_sample_delay);
    uint16_t x2 = adc_read();

    // If values are 0, there's no touch
    if(x1 < 20 || x2 < 20) {
        return 1000;
    }

    // Convert raw readings to pixel values
    x1 = x1 / 10.0;
    x1 = x1 - 30;

    x2 = x2 / 10.1;
    x2 = x2 - 50;
    x2 = 320 - x2;

    return (x1 + x2) / 2;
}

int touch_getY() {
    // Set up pins. YU and YD driving, XR input, XL tristated + pulldown
    gpio_set_dir(touch_YD, true);
    gpio_set_drive_strength(touch_YD, GPIO_DRIVE_STRENGTH_12MA);

    gpio_init(touch_YU);
    gpio_set_dir(touch_YU, true);
    gpio_set_drive_strength(touch_YU, GPIO_DRIVE_STRENGTH_12MA);

    gpio_set_dir(touch_XL, false);
    gpio_pull_down(touch_XL);

    adc_gpio_init(touch_XR);
    adc_select_input(touch_YIN);

    // Get reading 1
    gpio_put(touch_YU, false);
    gpio_put(touch_YD, true);

    sleep_us(touch_sample_delay);
    uint16_t y1 = adc_read();

    // Get reading 2 with reverse polarity
    gpio_put(touch_YU, true);
    gpio_put(touch_YD, false);

    sleep_us(touch_sample_delay);
    uint16_t y2 = adc_read();

    // If values are 0, there's no touch
    if(y1 < 20 || y2 < 20) {
        return 1000;
    }

    // Convert raw readings to pixel values
    y1 = y1 / 13;
    y1 = y1 - 35;

    y2 = y2 / 13;
    y2 = y2 - 30;
    y2 = 240 - y2;

    return (y1 + y2) / 2;
}

void touch_init() {
    gpio_init(touch_XL);
    gpio_init(touch_YD);

    gpio_set_dir(touch_XL, true);
    gpio_set_dir(touch_YD, true);

    // Enable pulldowns on touch_XN and touch_YN
    gpio_pull_down(touch_XL);
    gpio_pull_down(touch_YD);

    adc_init();
}