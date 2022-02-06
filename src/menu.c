#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pico/stdlib.h"
#include "menu.h"
#include "menu_defs.h"
#include "graphics.h"
#include "LCD_interface.h"
#include "actions.h"
#include "flash_interface.h"

/* The main file for defining the menu structure of the LCD interface. Instructions for adding menus
can be found in the README.md file.
*/

void menu_HOME_SCREEN_render() {
    currentMenu = HOME_SCREEN;

    // Background
    LCD_rect(0, 0, 320, 240, 0xFFFFFF);

    // Header
    LCD_rect(0, 0, 320, 20, 0x000000);
    graphics_text(10, 4, "Main Menu", 1.5, 0xFFFFFF, 0x000000);
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    // Settings button
    LCD_rect(260, 180, 50, 50, 0x000000);
    graphics_icon(265, 185, SETTINGS_GEAR, 2);

    // Actuator Control
    LCD_rect(10, 50, 280, 50, 0x000000);
    graphics_text(15, 68, "Actuator Control", 2, 0xFFFFFF, 0x000000);

    // Active zones
    actions_clearLinks();
    actions_addLink(260, 180, 50, 50, SETTINGS_MAIN);
    actions_addLink(10, 50, 280, 50, ACTUATOR_MAIN);
}

void menu_HOME_SCREEN_update() {
    // Redraw battery icon
    graphics_icon(300, 0, BATTERY_WIDGET, 1);
}

void menu_SETTINGS_MAIN_render() {
    currentMenu = SETTINGS_MAIN;

    // Background
    LCD_rect(0, 0, 320, 240, 0xFFFFFF);

    // Header
    LCD_rect(0, 0, 320, 20, 0x000000);
    graphics_text(10, 4, "Settings", 1.5, 0xFFFFFF, 0x000000);
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    // Back button
    LCD_rect(260, 180, 50, 50, 0x000000);
    graphics_icon(265, 185, BACK_ARROW, 2);

    // Actuator Settings
    LCD_rect(10, 50, 280, 50, 0x000000);
    graphics_text(15, 68, "Actuator Settings", 2, 0xFFFFFF, 0x000000);

    // Active Zones
    actions_clearLinks();
    actions_addLink(260, 180, 50, 50, HOME_SCREEN);
    actions_addLink(10, 50, 280, 50, SETTINGS_ACTUATOR);
}

void menu_SETTINGS_MAIN_update() {
    // Redraw battery icon
    graphics_icon(300, 0, BATTERY_WIDGET, 1);
}

void menu_ACTUATOR_MAIN_render() {
    currentMenu = ACTUATOR_MAIN;

    // Background
    LCD_rect(0, 0, 320, 240, 0xFFFFFF);

    // Header
    LCD_rect(0, 0, 320, 20, 0x000000);
    graphics_text(10, 4, "Actuator Control", 1.5, 0xFFFFFF, 0x000000);
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    // Back button
    LCD_rect(260, 180, 50, 50, 0x000000);
    graphics_icon(265, 185, BACK_ARROW, 2);

    // Actuator Position
    LCD_rect(50, 50, 100, 40, 0x000000);
    graphics_text(52, 52, "Position", 1.3, 0x00FF00, 0x000000);
    graphics_value(52, 73, ACTUATOR_POSITION, 2, 0x00FF00, 0x000000);

    // Actuator Force
    LCD_rect(170, 50, 100, 40, 0x000000);
    graphics_text(172, 52, "Force", 1.3, 0xFF0000, 0x000000);
    graphics_value(172, 73, ACTUATOR_FORCE, 2, 0xFF0000, 0x000000);

    // Actuator Control Buttons
    graphics_text(30, 100, "Position Control", 1.5, 0x000000, 0xFFFFFF);

    LCD_rect(10, 130, 160, 40, 0x000000);
    graphics_text(12, 134, "Pos", 1.5, 0xFFFFFF, 0x000000);
    graphics_text(12, 149, "Set:", 1.5, 0xFFFFFF, 0x000000);
    graphics_value(85, 144, ACTUATOR_SETPOINT, 2, 0xFFFFFF, 0x000000);

    // Setpoint Inc/Dec
    // +
    LCD_rect(200, 130, 40, 40, 0x00FF00);
    LCD_rect(205, 145, 30, 10, 0x000000);
    LCD_rect(215, 135, 10, 30, 0x000000);
    // -
    LCD_rect(250, 130, 40, 40, 0xFF0000);
    LCD_rect(255, 145, 30, 10, 0x000000);


    // Active zones
    actions_clearLinks();
    actions_addLink(260, 180, 50, 50, HOME_SCREEN);
    actions_addAction(200, 130, 40, 40, ACTUATOR_FOR);
    actions_addAction(250, 130, 40, 40, ACTUATOR_REV);
}

void menu_ACTUATOR_MAIN_update() {
    // Redraw battery icon
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    graphics_value(52, 73, ACTUATOR_POSITION, 2, 0x00FF00, 0x000000);
    graphics_value(172, 73, ACTUATOR_FORCE, 2, 0xFF0000, 0x000000);
    graphics_value(85, 144, ACTUATOR_SETPOINT, 2, 0xFFFFFF, 0x000000);
}

void menu_SETTINGS_ACTUATOR_render() {
    currentMenu = SETTINGS_ACTUATOR;

    // Background
    LCD_rect(0, 0, 320, 240, 0xFFFFFF);

    // Header
    LCD_rect(0, 0, 320, 20, 0x000000);
    graphics_text(10, 4, "Actuator Settings", 1.5, 0xFFFFFF, 0x000000);
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    // Back button
    LCD_rect(260, 180, 50, 50, 0x000000);
    graphics_icon(265, 185, BACK_ARROW, 2);

    // Force control
    LCD_rect(10, 30, 160, 40, 0x000000);
    graphics_text(12, 34, "Force", 1.5, 0xFFFFFF, 0x000000);
    graphics_text(12, 49, "Limit:", 1.5, 0xFFFFFF, 0x000000);
    graphics_setting(85, 44, ACTUATOR_FORCE_LIMIT, 2, 0xFFFFFF, 0x000000);

    // Force Inc/Dec
    // +
    LCD_rect(200, 30, 40, 40, 0x00FF00);
    LCD_rect(205, 45, 30, 10, 0x000000);
    LCD_rect(215, 35, 10, 30, 0x000000);
    // -
    LCD_rect(250, 30, 40, 40, 0xFF0000);
    LCD_rect(255, 45, 30, 10, 0x000000);

    // Speed control
    LCD_rect(10, 80, 160, 40, 0x000000);
    graphics_text(12, 84, "Speed", 1.5, 0xFFFFFF, 0x000000);
    graphics_text(12, 99, "Limit:", 1.5, 0xFFFFFF, 0x000000);
    graphics_setting(85, 94, ACTUATOR_SPEED_LIMIT, 2, 0xFFFFFF, 0x000000);

    // Speed Inc/Dec
    // +
    LCD_rect(200, 80, 40, 40, 0x00FF00);
    LCD_rect(205, 95, 30, 10, 0x000000);
    LCD_rect(215, 85, 10, 30, 0x000000);
    // -
    LCD_rect(250, 80, 40, 40, 0xFF0000);
    LCD_rect(255, 95, 30, 10, 0x000000);

    // Time Delay control
    LCD_rect(10, 130, 160, 40, 0x000000);
    graphics_text(12, 134, "Time", 1.5, 0xFFFFFF, 0x000000);
    graphics_text(12, 149, "Delay:", 1.5, 0xFFFFFF, 0x000000);
    graphics_setting(85, 144, ACTUATOR_TIME_DELAY, 2, 0xFFFFFF, 0x000000);

    // Delay Inc/Dec
    // +
    LCD_rect(200, 130, 40, 40, 0x00FF00);
    LCD_rect(205, 145, 30, 10, 0x000000);
    LCD_rect(215, 135, 10, 30, 0x000000);
    // -
    LCD_rect(250, 130, 40, 40, 0xFF0000);
    LCD_rect(255, 145, 30, 10, 0x000000);

    // Active Zones
    actions_clearLinks();
    actions_addLink(260, 180, 50, 50, SETTINGS_MAIN); // Back arrow destination
    actions_addSetting(200, 30, 40, 40, ACTUATOR_FORCE_LIMIT, INCREMENT);
    actions_addSetting(250, 30, 40, 40, ACTUATOR_FORCE_LIMIT, DECREMENT);

    actions_addSetting(200, 80, 40, 40, ACTUATOR_SPEED_LIMIT, INCREMENT);
    actions_addSetting(250, 80, 40, 40, ACTUATOR_SPEED_LIMIT, DECREMENT);

    actions_addSetting(200, 130, 40, 40, ACTUATOR_TIME_DELAY, INCREMENT);
    actions_addSetting(250, 130, 40, 40, ACTUATOR_TIME_DELAY, DECREMENT);
}

void menu_SETTINGS_ACTUATOR_update() {
    // Redraw battery icon
    graphics_icon(300, 0, BATTERY_WIDGET, 1);

    // Redraw setting values
    graphics_setting(85, 44, ACTUATOR_FORCE_LIMIT, 2, 0xFFFFFF, 0x000000);
    graphics_setting(85, 94, ACTUATOR_SPEED_LIMIT, 2, 0xFFFFFF, 0x000000);
    graphics_setting(85, 144, ACTUATOR_TIME_DELAY, 2, 0xFFFFFF, 0x000000);
}