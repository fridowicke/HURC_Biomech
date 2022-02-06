# The Dorsiflexer Design
- Note that some of this has already been covered in the README, since some aspects of the code layout are relevant to the user

## Overview

The project layout consists of a framework made of interface files and controller files supporting a menu definitions file, which contains high-level descriptions of each menu page on the device. This is implemented in a series of functions in `menu.c`, two for each menu page. One function is the initial render function, which draws all of the objects making up the menu when it is initially rendered. The other function is the update function, which is called on a regular basis to update menu elements like sensor displays so that they show the latest measurement.

## menu.c and its helper files

The functions in `menu.c` utilize helper functions defined in `actions.c` and `graphics.c`, which define methods for reacting to user input and generating graphics, respectively. `actions.c` is implemented via an array of structs, each of which corresponds to an area on the screen and a function to run when that area is tapped. A function `actions_testAction()` is called on a regular basis with the x and y coordinates of a touch event, and runs through the array of actionable areas until it either finds one corresponding to the touched area, or runs out of actionable areas. If it finds one, it will call `actions_executeAction()`, which will run appropriate functions to execute the action associated with the actionable areas. This functionality is exposed to `menu.c` in terms of a few functions. `actions_clearLinks()` clears the list of actionable areas, and is called whenever a new menu is drawn in order to reset the buttons. `actions_addAction()`, `actions_addSetting()`, and `actions_addLink()` all find an empty slot in the array to put a new actionable area in.

`graphics.c` is implemented as several functions calling lower-level functions defined in `LCD_interface.c`. The functions exposed to `menu.c` include `graphics_text()`, `graphics_value()`, `graphics_setting()`, and `graphics_icon()`, which all work by allocating a buffer to hold image data, filling it with something, and then passing that image buffer to the `LCD_interface.c` library. `graphics.c` also includes a function to resize a graphics buffer, using the closest-pixel method for simplicity and speed.

## LCD_interface.c

`LCD_interface.c` defines the interface between the higher-level graphics libraries and the hardware signals that comprise communication between the Pico and the LCD. This is implemented via two helper functions, `LCD_cmd()` and `LCD_dat()`, which write a byte to the LCD while indicating which type of information it is. The `LCD_init()` function is called on startup, and sets up the pins and PIO state machine for communication. More info on the PIO in a moment. These helper functions are called by `LCD_window`, `LCD_rect`, and `LCD_buffer`. The first tells the LCD that we are only editing a small rectangular area of the display, so the LCD will automatically map the data we transmit to that subset of the larger screen. The second two functions utilize the first to define a working area, and then fill it with either a solid color or the contents of a buffer.

## PIO state machine

More information about the PIOs is available online, but they are essentially very basic secondary processors, programmed in assembly code, that can take care of the particulars of implementing protocols, leaving the main processor free to deal with more pressing matters. In this case, one is being set up to take in bytes of data and present them on a parallel interface while toggling a write strobe to indicate to the LCD that a new byte of data is incoming. Data is passed to the state machine by `LCD_cmd()` and `LCD_dat()`.

## flash_interface.c

`flash_interface.c` implements a set of functions related to storing data persistently across reboots. Since the Pico's ram is erased when power is removed, we need to store our data in the offboard non-volatile flash. Unfortunately, we can't write to the flash in a bytewise manner, so we need to write the whole settings section whenever we want to update settings. This file abstracts away the particulars of that, making it appear as though we can change specific setting values at will.

## actuator_interface.c

Since part of the electronics design involved designing my own motor controller, extra code is needed to manually PWM certain outputs and ensure that no cross-conduction occurs, as that tends to melt electronics. `actuator_interface.c` implements the specifics of these rules, allowing `actuator_control.c` to simply set motor power when it needs to instead of having to fuddle with duty cycles and pin values.
It also implements hardware interfaces with the position and force sensors, exposing data from those to `actuator_control.c` and `graphics.c` in the form of `actuator_get_force()` and `actuator_get_position()`.

## actuator_control.c

`actuator_control.c` implements a simple bang-bang closed-loop controller, using data from `actuator_get_position()` and an internal `target_position` to run the motor at 30% power toward the setpoint. It exposes the setpoint to `actions.c` in the form of `motorControl_setTarget()` and `motorControl_getTarget()` so that the setpoint can be changed through touch events.

## touch_interface.c

This file implements the scanning functionality of a resistive touch interface, reading the analog data from the resistive panel and translating it into pixel values. The functions `touch_getX()` and `touch_getY()` both return 1000 when no touch is detected. `actions_testAction()` doesn't respond to pixel values outside of the display.

## icons.h

This file includes some simple bitmaps I put together to indicate settings and battery status. It is used by `graphics_icon()` to fill a data buffer.

## basic_LCD.c

This file contains the `main()` function for the code, and calls all of the `init` functions necessary to set up all of the interfaces before entering an infinite loop scanning for touch events, updating the menu, and recalculating actuator output.

## A technical note about mallocs

We were taught in CS50 to always check the pointer returned by malloc, as it would return a null pointer if there somehow wasn't enough memory to allocate. We did this to avoid causing bigger issues with the computer, like causing segfaults and the like. In this project, all memory allocations are program-critical, so if there somehow wasn't enough memory to allocate, crashing the program would be equivalent to checking for a null pointer and ending the program, since this is running on bare metal.
