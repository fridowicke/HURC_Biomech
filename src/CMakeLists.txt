cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH "/home/ben/Desktop/HURC/pico/pico-sdk")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(basic_LCD C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(basic_LCD
    basic_LCD.c
    LCD_interface.c
    touch_interface.c
    graphics.c
    menu.c
    actuator_interface.c
    actions.c
    flash_interface.c
    actuator_control.c
)

# pico_enable_stdio_usb(basic_LCD 1)
# pico_enable_stdio_uart(basic_LCD 0)

pico_set_program_name(basic_LCD "basic_LCD")
pico_set_program_version(basic_LCD "0.1")

# Generate PIO header for program and initialization function inclusion
pico_generate_pio_header(basic_LCD ${CMAKE_CURRENT_LIST_DIR}/basic_LCD.pio)

target_sources(basic_LCD
    PRIVATE basic_LCD.c
)

# Add the necessary libraries to the build
target_link_libraries(basic_LCD
    pico_stdlib
    pico_malloc
    pico_mem_ops
    pico_runtime
    hardware_pwm
    hardware_pio
    hardware_adc
    hardware_flash
    hardware_sync
    hardware_i2c
)

pico_add_extra_outputs(basic_LCD)