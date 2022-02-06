#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LCD_interface.h"
#include "touch_interface.h"
#include "graphics.h"
#include "menu.h"
#include "actuator_interface.h"
#include "actions.h"
#include "flash_interface.h"
#include "actuator_control.h"

int main() {
    // Init stdio for debugging purposes, and ensure system clock speed is 130 MHz
    stdio_init_all();
    set_sys_clock_khz(130000, true);

    // Initialize interfaces, trackers, and controllers
    LCD_init();
    touch_init();
    actuator_init();
    actions_init();
    flash_init();
    motorControl_init();

    // Draw home screen on the LCD
    menu_HOME_SCREEN_render();

    uint16_t x = 0, y = 0;

    // Main program loop at 20 Hz
    while(true) {
        // Check for a touch on the screen
        x = touch_getX();
        y = touch_getY();
        
        // If a touch was detected, take action
        actions_testAction(x, y);

        // Update refreshable items on screen
        actions_updateMenu();

        // Calculate and apply actuator output
        motorControl_run();
    
        sleep_ms(50);
    }
}