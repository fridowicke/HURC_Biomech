#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "flash_interface.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"

void flash_init() {
    // Offset of settings data 256kfrom start of flash
    settingsFlashOffset = 256 * 1024;

    // ARM memory address of flash data
    flashSettingsTarget = (uint8_t *)(XIP_BASE + settingsFlashOffset);

    // Buffer for a local copy of the settings data
    settingsBuffer = calloc(FLASH_PAGE_SIZE, sizeof(uint8_t));
    
    // Set local copy to flash storage
    memcpy(settingsBuffer, flashSettingsTarget, FLASH_PAGE_SIZE);
}

void flash_update_setting(settings setting, uint16_t new_value) {
    // Update the local buffer with the new setting value
    settingsBuffer[(uint8_t)setting] = new_value >> 8; // Upper half
    settingsBuffer[(uint8_t)setting + 1] = new_value & 0xFF; // Lower half

    // Write local buffer to flash. Interrupts need to be disabled while writing
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(settingsFlashOffset, 4096);
    flash_range_program(settingsFlashOffset, settingsBuffer, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
}

uint16_t flash_get_setting(settings setting) {
    // Pull 16-bit value out of 2 8-bit fields in local buffer
    uint16_t retval;
    retval = (settingsBuffer[(uint8_t)setting] << 8) | settingsBuffer[(uint8_t)setting + 1]; // Upper half + lower half

    return retval;
}