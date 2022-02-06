#ifndef menu_defs_h
#define menu_defs_h

#include "flash_interface.h"

#define NONE 0

// Enums for defining menu objects in menu.c
typedef enum menuID {
    HOME_SCREEN = 1,
    SETTINGS_MAIN = 2,
    ACTUATOR_MAIN = 3,
    SENSOR_MAIN = 4,
    SETTINGS_ACTUATOR = 5
} menuID;

typedef enum menuAction {
    POWER_OFF = 1,
    ACTUATOR_STOP = 2,
    ACTUATOR_FOR = 3,
    ACTUATOR_REV = 4,
    RENDER_MENU = 5,
    UPDATE_SETTING = 6
} menuAction;

typedef enum valueID {
    TOUCH_X = 1,
    TOUCH_Y = 2,
    VBAT = 3,
    ACTUATOR_POSITION = 4,
    ACTUATOR_FORCE = 5,
    ACTUATOR_SETPOINT = 6
} valueID;

typedef enum iconType {
    SETTINGS_GEAR = 1,
    BATTERY_WIDGET = 2,
    BACK_ARROW = 3
} iconType;

// Struct for tracking actionable areas on the LCD
typedef struct menuLink {
    bool active; // True if this area should be actionable
    uint16_t X;
    uint16_t Y;
    uint16_t width;
    uint16_t height;
    menuAction action;
    menuID menu; // Only relevant if action is RENDER_MENU
    settings setting; // Only relevant if action is UPDATE_SETTING
    settingActions settingAction; // Only relevant if action is UPDATE_SETTING
} menuLink;

#endif