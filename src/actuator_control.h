#ifndef actuator_control_h
#define actuator_control_h

uint16_t target_position; // Actuator target position in mm

//! \brief Initialize motor controller
void motorControl_init();

//! \brief Calculate motor control output
void motorControl_run();

//! \brief Set controller position target
void motorControl_setTarget(uint16_t target);

//! \brief Return controller position target
uint16_t motorControl_getTarget();

#endif