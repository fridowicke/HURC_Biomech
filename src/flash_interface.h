#ifndef flash_interface_h
#define flash_interface_h

typedef enum settings { // Define offsets for settings. All settings are 16-bit
    ACTUATOR_SPEED_LIMIT = 2,
    ACTUATOR_FORCE_LIMIT = 4,
    ACTUATOR_TIME_DELAY = 6
} settings;

typedef enum settingActions {
    INCREMENT = 1,
    DECREMENT = 2
} settingActions;

uint32_t settingsFlashOffset;

uint8_t* settingsBuffer;
uint8_t* flashSettingsTarget;

//! \brief Initialize local copy of flash settings
void flash_init();

//! \brief Update local copy of setting and write to flash
void flash_update_setting(settings setting, uint16_t new_value);

//! \brief Return setting value from local data
uint16_t flash_get_setting(settings setting);

#endif