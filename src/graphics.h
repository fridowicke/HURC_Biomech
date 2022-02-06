#ifndef graphics_h
#define graphics_h

#include "pico/stdlib.h"
#include "menu_defs.h"

//! \brief Display text on the screen
void graphics_text(uint16_t x, uint16_t y, char *string, float size, uint32_t color, uint32_t backgroundColor);

//! \brief Scale an image buffer by x and y factors
uint8_t* graphics_scale(uint8_t* buf, uint16_t width, uint16_t height, float xfactor, float yfactor);

//! \brief Display a sensor value to the screen
void graphics_value(uint16_t x, uint16_t y, valueID id, float size, uint32_t color, uint32_t backgroundColor);

//! \brief Display a current setting value
void graphics_setting(uint16_t x, uint16_t y, settings setting, float size, uint32_t color, uint32_t backgroundColor);

//! \brief Display an icon
void graphics_icon(uint16_t x, uint16_t y, iconType icon, float size);

#endif