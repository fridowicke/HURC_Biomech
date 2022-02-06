# The Dorsiflexer Implementation

Project Video: [https://youtu.be/1brG92CK5NE](https://youtu.be/1brG92CK5NE)

## Project Background

The purpose of this project is to provide a suitable control framework for the Dorsiflexer, a device designed to combat ankle spasticity in post-stroke patients. These patients are often bedridden, unable to walk. Without regular exercise of the ankle muscles, the muscles and tendons tend to seize up and cause problems ranging from difficulty walking to muscular deformities. The traditional treatment for ankle spasticity is to repeatedly move the patient's ankle through its full range of motion over the course of a few hours each day in order to keep the muscles flexible. This is normally done by a physical trainer, but under-resourced facilities are often unable to provide every patient with a trainer for extended lengths of time. The goal of this project was to develop control software for a device that can perform this task at home or in a clinic following instructions from a technician.

## Project Overview

The device itself consists primarily of a motorized linear actuator with an integrated sensor that can measure extension, a load cell in line with the actuator to measure the amount of pressure it is exerting, and a touchscreen to interface with the patient and the technician. All of these are controlled by a Raspberry Pi Pico microcontroller, programmed in C. This project consists of a framework made of interface files and controller files supporting a menu definitions file, which contains the high-level descriptions of each menu page on the device. Instructions for adding menus are covered in `Adding Menus`.

## Compilation and Upload

This project is compiled using the standard [Raspberry Pi Pico Toolchain](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) (Instructions in the section labeled "The SDK"). I used VSCode for editing, as is suggested in the guide, as it allows easy integration with IntelliSense. Following the instructions in that guide to download the SDK and install VSCode extensions should allow for compilation of this code into a .uf2, which can then be uploaded to the Pi by holding down the BootSel button and plugging it into the computer. This will cause the Pi to appear as a standard flash drive, and the built .uf2 file can be copied onto the Pi. Be careful when installing the toolchain - ensure that the PICO_SDK_PATH environment variable is correctly set the first time, as an incorrect setting will be incorporated into several configuration files and require the full reinstallation of the entire toolchain. Ask me how I know.

## Adding menus
Once the toolchain has been installed, menu definitions can be added. To add a new static menu, start by adding a menu identifier `NAMEOFMENU` to the `menuID` enum in `menu_defs.h`, giving it a new number. Then, create a new function in the `menu.c` file called `menu_[NAMEOFMENU]_render()`. Start with a line `currentMenu = NAMEOFMENU;` to change the `currentMenu` variable when the menu is rendered. Now, add graphics components as you see fit, usually starting with an LCD_rect() to provide a background and then adding text or icons on top of that. Once that function definition is complete, add a function template for the new function to the `menu.h` header, and add a case to the switch statement in `actions_renderMenu()` in `actions.c` calling `menu_[NAMEOFMENU]_render()` when `NAMEOFMENU` is passed into the switch.

If the new menu has dynamic elements, it is also necessary to add a `menu_[NAMEOFMENU]_update()` function to `menu.c`, which re-renders any dynamic elements in the menu when called. This function must also be added as a template to `menu.h`, and a case must be added to the switch statement in `actions_updateMenu()` calling `menu_[NAMEOFMENU]_update()` when `NAMEOFMENU` is passed into the switch.

If any of that was confusing, there is already an example menu system implemented in the framework that can be modified or replaced.

## Function documentation
This framework adds several graphics helper functions intended for use in menu definitions in `menu.c`. These abstract away the nasty bits of dealing with pins and protocols and real-world devices, allowing, for example, sensor readings to be displayed painlessly through unified functions like `graphics_value()`. Descriptions of each of the helper functions I've implemented:

* Shape Primitives
  * `LCD_rect(x, y, width, height, color)`
    * Draws a `color` rectangle at `x`, `y` of dimensions `width` x `height`. Color is a 24-bit int, of which the first 8 bits refer to the red value, the second 8 bits to the green value, and the last 8 bits to the blue value.
* Graphics Primitives
  * `graphics_text(x, y, string, size, color, backgroundColor)`
    * Draws text `string` at `x`, `y` of `color` over `backgroundColor`. `size` is a float defining the size of the text - 1 is 8 pixels tall, 2 is 16px, 2.5 is 20px, and so forth.
  * `graphics_icon(x, y, icon, size)`
    * Draws an `icon` at `x`, `y` in black and white of `size`, which works the same way as text except the default icon size (size 1) is 20x20 pixels. Possible values for `icon` can be found in the `iconType` enum in `menu_defs.h`.
  * `graphics_value(x, y, value, size, color, backgroundColor)`
    * Measures and displays the sensor value indicated by `value` at `x`, `y` of `color` over `backgroundColor`. Possible values for `value` can be found in the `valueID` enum in `menu_defs.h`.
  * `graphics_setting(x, y, setting, size, color, backgroundColor)`
    * Measures and displays the setting value indicated by `setting` at `x`, `y` of `color` over `backgroundColor`. Possible values for `setting` can be found in the `settings` enum in `flash_interface.h`.
* Actions
  * `actions_clearLinks()`
    * Removes all instances of touch-sensitive areas from the display
  * `actions_addAction(x, y, width, height, action)`
    * Adds touch-sensitive area defined by `x`, `y`, `width`, and `height` which, when clicked, will execute `action`. Possible values for `action` can be found in the `menuAction` enum in `menu_defs.h`.
  * `actions_addLink(x, y, width, height, menu)`
    * Adds touch-sensitive area defined by `x`, `y`, `width`, and `height` which, when clicked, will go to destination menu `menu`. Possible values for `menu` can be found in the `menuID` enum in `menu_defs.h`.
  * `actions_addSetting(x, y, width, height, setting, settingAction)`
    * Adds touch-sensitive area defined by `x`, `y`, `width`, and `height` which, when clicked, will change `setting` by `settingAction`. Possible values for `setting` and `settingAction` can be found in corresponding enums in `flash_interface.h`.

Once again, examples of all of these functions are present in the demonstration menu system already implemented!

Good luck with your project!
