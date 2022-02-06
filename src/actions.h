#ifndef actions_h
#define actions_h

#define MAX_LINKS 10

#include "menu_defs.h"

menuLink* linkStack;

menuID currentMenu;

void actions_init();

void actions_clearLinks();

//! \brief Add action to action list
uint8_t actions_addAction(uint16_t x, uint16_t y, uint16_t width, uint16_t height, menuAction action);

//! \brief Add link to another menu
uint8_t actions_addLink(uint16_t x, uint16_t y, uint16_t width, uint16_t height, menuID menu);

//! \brief Add setting change button
uint8_t actions_addSetting(uint16_t x, uint16_t y, uint16_t width, uint16_t height, settings setting, settingActions settingAction);

//! \brief Execute action corresponding to point (x, y)
uint8_t actions_testAction(uint16_t x, uint16_t y);

//! \brief Execute action of activeLink
void actions_executeAction(menuLink activeLink);

//! \brief Call rendering function corresponding to menu
void actions_renderMenu(menuID menu);

//! \brief Call updating function for current menu
void actions_updateMenu();

//! \brief Change value of setting
void actions_updateSetting(settings setting, settingActions settingAction);

#endif