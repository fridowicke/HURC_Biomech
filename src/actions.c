#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/malloc.h"
#include "menu.h"
#include "actions.h"
#include "menu_defs.h"
#include "graphics.h"
#include "LCD_interface.h"
#include "actuator_interface.h"
#include "flash_interface.h"
#include "actuator_control.h"

// Controller file for interpreting touch events on the touchscreen and calling the necessary function

void actions_init() {
    // Allocate stack for ten tappable buttons
    linkStack = malloc(sizeof(menuLink) * MAX_LINKS);
    actions_clearLinks();
}

void actions_clearLinks() {
    // Clear all 'active' fields in linkStack
    for(uint8_t i = 0; i < MAX_LINKS; i++) {
        linkStack[i].active = false;
    }
}

uint8_t actions_addAction(uint16_t x, uint16_t y, uint16_t width, uint16_t height, menuAction action) {
    uint8_t i = 0;

    // Find first empty stack slot
    while(true) {
        if(linkStack[i].active == false) {
            break;
        }
        i++;
        if(i == MAX_LINKS) { // No room in the stack for another active region
            return 1;
        }
    }

    // Set fields in empty stack slot
    linkStack[i].active = true;
    linkStack[i].X = x;
    linkStack[i].Y = y;
    linkStack[i].width = width;
    linkStack[i].height = height;
    linkStack[i].action = action;
    linkStack[i].menu = NONE;
    linkStack[i].setting = NONE;
    linkStack[i].settingAction = NONE;

    return 0; // Successfully added active region
}

uint8_t actions_addLink(uint16_t x, uint16_t y, uint16_t width, uint16_t height, menuID menu) {
    uint8_t i = 0;

    // Find first empty stack slot
    while(true) {
        if(linkStack[i].active == false) {
            break;
        }
        i++;
        if(i == MAX_LINKS) { // No room in the stack for another active region
            return 1;
        }
    }

    // Set fields in empty stack slot
    linkStack[i].active = true;
    linkStack[i].X = x;
    linkStack[i].Y = y;
    linkStack[i].width = width;
    linkStack[i].height = height;
    linkStack[i].action = RENDER_MENU;
    linkStack[i].menu = menu;
    linkStack[i].setting = NONE;
    linkStack[i].settingAction = NONE;

    return 0; // Successfully added active region
}

uint8_t actions_addSetting(uint16_t x, uint16_t y, uint16_t width, uint16_t height, settings setting, settingActions settingAction) {
    uint8_t i = 0;

    // Find first empty stack slot
    while(true) {
        if(linkStack[i].active == false) {
            break;
        }
        i++;
        if(i == MAX_LINKS) { // No room in the stack for another active region
            return 1;
        }
    }

    // Set fields in empty stack slot
    linkStack[i].active = true;
    linkStack[i].X = x;
    linkStack[i].Y = y;
    linkStack[i].width = width;
    linkStack[i].height = height;
    linkStack[i].action = UPDATE_SETTING;
    linkStack[i].menu = NONE;
    linkStack[i].setting = setting;
    linkStack[i].settingAction = settingAction;

    return 0; // Successfully added active region
}

uint8_t actions_testAction(uint16_t x, uint16_t y) {
    // Check input for valid point
    if(x > 319 || y > 239) {
        return 1; // Invalid point
    }

    uint8_t i = 0;

    // Find first link with region containing input point.
    while(true) {
        // Ensure link is active
        if(linkStack[i].active == true) {
            // Check x
            if(x > linkStack[i].X && x < (linkStack[i].X + linkStack[i].width)) {
                // Check y
                if(y > linkStack[i].Y && y < (linkStack[i].Y + linkStack[i].height)) {
                    break;
                }
            }
        }
        i++;
        if(i == MAX_LINKS) {
            return 2; // No link found for that point
        }
    }

    // Execute the action
    actions_executeAction(linkStack[i]);
}

void actions_executeAction(menuLink activeLink) {
    if(activeLink.action == NONE) {
        return;
    }

    // Identify the action to be taken
    switch(activeLink.action) {
        case POWER_OFF:
            break;
        case ACTUATOR_STOP:
            break;
        case ACTUATOR_FOR:
            motorControl_setTarget(motorControl_getTarget() + 10);
            break;
        case ACTUATOR_REV:
            motorControl_setTarget(motorControl_getTarget() - 10);
            break;
        case RENDER_MENU:
            actions_renderMenu(activeLink.menu);
            break;
        case UPDATE_SETTING:
            actions_updateSetting(activeLink.setting, activeLink.settingAction);
            break;
    }
}

void actions_renderMenu(menuID menu) {
    // Render the menu
    switch(menu) {
        case HOME_SCREEN:
            menu_HOME_SCREEN_render();
            break;
        case SETTINGS_MAIN:
            menu_SETTINGS_MAIN_render();
            break;
        case ACTUATOR_MAIN:
            menu_ACTUATOR_MAIN_render();
            break;
        case SETTINGS_ACTUATOR:
            menu_SETTINGS_ACTUATOR_render();
            break;
        case SENSOR_MAIN:
            break;
    }
}

void actions_updateMenu() {
    // Update the current menu
    switch(currentMenu) {
        case HOME_SCREEN:
            menu_HOME_SCREEN_update();
            break;
        case SETTINGS_MAIN:
            menu_SETTINGS_MAIN_update();
            break;
        case ACTUATOR_MAIN:
            menu_ACTUATOR_MAIN_update();
            break;
        case SETTINGS_ACTUATOR:
            menu_SETTINGS_ACTUATOR_update();
            break;
    }
}

void actions_updateSetting(settings setting, settingActions settingAction) {
    uint16_t settingValue = flash_get_setting(setting);

    // Change indicated setting
    switch(settingAction) {
        case INCREMENT: // Note rolling over settings is ok
            settingValue++;
            break;
        case DECREMENT:
            settingValue--;
            break;
    }

    flash_update_setting(setting, settingValue);
}