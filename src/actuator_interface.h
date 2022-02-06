#ifndef actuator_interface_h
#define actuator_interface_h

#define act_FB 28 // Analog feedback from actuator

#define AH 14 // A high
#define AL 15 // A low
#define BH 16 // B high
#define BL 17 // B low

// Dat/clk lines from load cell
#define HX711_DAT 18
#define HX711_CLK 19

#define ACTUATOR_POSITION_SAMPLES 5

// PWM slice numbers for motor PWM
uint8_t A_slice_num;
uint8_t B_slice_num;

//! \brief Init actuator interface and set up pins/timers
void actuator_init();

//! \brief Set actuator power
void actuator_set_power(uint16_t power, bool dir);

//! \brief Get position data from actuator
uint16_t actuator_get_position();

//! \brief Read force data from load cell
uint16_t actuator_get_force();

#endif