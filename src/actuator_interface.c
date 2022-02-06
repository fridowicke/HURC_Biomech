#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "actuator_interface.h"

void actuator_init() {
    // Set up sensor feedback
    adc_gpio_init(act_FB);

    // Set up output drivers
    // Set up low-side GPIO interfaces
    gpio_init(AL);
    gpio_init(BL);

    gpio_set_dir(AL, true);
    gpio_set_dir(BL, true);

    gpio_put(AL, false);
    gpio_put(BL, false);

    // Init high-side GPIOs as PWM
    gpio_set_function(AH, GPIO_FUNC_PWM); // 7A
    gpio_set_function(BH, GPIO_FUNC_PWM); // 0A

    A_slice_num = pwm_gpio_to_slice_num(AH);
    B_slice_num = pwm_gpio_to_slice_num(BH);

    // Set clock divs on both slices
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, 6);

    pwm_init(A_slice_num, &cfg, false);
    pwm_init(B_slice_num, &cfg, false);

    // Set wrap values to 1000 and set thresholds to 0, always off
    pwm_set_wrap(A_slice_num, 1000);
    pwm_set_wrap(B_slice_num, 1000);

    pwm_set_chan_level(A_slice_num, PWM_CHAN_A, 0);
    pwm_set_chan_level(B_slice_num, PWM_CHAN_A, 0);

    pwm_set_mask_enabled(0b10000001); // Enable slices A and B simultaneously


    // Set up force feedback
    gpio_init(HX711_DAT);
    gpio_init(HX711_CLK);
    gpio_set_dir(HX711_DAT, false);
    gpio_set_dir(HX711_CLK, true);
    gpio_put(HX711_CLK, false);

    actuator_set_power(0, true);
}

// Set power from 0 to 1000. dir=true for forward
void actuator_set_power(uint16_t power, bool dir) {
    if(power == 0) { // Disable everything (coast)
        pwm_set_chan_level(A_slice_num, PWM_CHAN_A, 0); // Disable A high
        pwm_set_chan_level(B_slice_num, PWM_CHAN_A, 0); // Disable B high

        gpio_put(AL, true); // Hold A low
        gpio_put(BL, true); // Hold B low
    }

    if(dir) { // Forward
        pwm_set_chan_level(B_slice_num, PWM_CHAN_A, 0); // Disable B
        gpio_put(AL, false); // Prepare to bring A high

        sleep_us(4); // Give mosfets time to turn off

        gpio_put(BL, true); // B side low
        pwm_set_chan_level(A_slice_num, PWM_CHAN_A, power); // Enable A
    } else { // Reverse
        pwm_set_chan_level(A_slice_num, PWM_CHAN_A, 0); // Disable A
        gpio_put(BL, false); // Prepare to bring B high

        sleep_us(4); // Give mosfets time to turn off

        gpio_put(AL, true); // A side low
        pwm_set_chan_level(B_slice_num, PWM_CHAN_A, power); // Enable B
    }
}

uint16_t actuator_get_position() {
    // Return position reading
    adc_select_input(2);
    uint32_t retval = 0;

    // Take the average of ACTUATOR_POSITION_SAMPLES samples
    for(uint8_t i = 0; i < ACTUATOR_POSITION_SAMPLES; i++) {
        retval += adc_read();
    }

    return retval / ACTUATOR_POSITION_SAMPLES;
}

uint16_t actuator_get_force() {
    uint32_t force = 0;

    while(gpio_get(HX711_DAT)); // Stall until DAT goes low

    // Shift in data
    for(uint8_t i = 0; i < 24; i++) {
        gpio_put(HX711_CLK, true);
        sleep_us(1);
        force = force | gpio_get(HX711_DAT) << (23 - i);
        gpio_put(HX711_CLK, false);
        sleep_us(1);
    }
    for(uint8_t i = 0; i < 3; i++) { // Wiggle CLK 3x to set gain to 64
        gpio_put(HX711_CLK, true);
        sleep_us(1);
        gpio_put(HX711_CLK, false);
        sleep_us(1);
    }

    force = force & 0xFFFF;

    return force;
}