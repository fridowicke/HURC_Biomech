#include <stdio.h>

#include "pico/stdlib.h"
#include "actuator_control.h"
#include "actuator_interface.h"

void motorControl_init() {
    // Start with position target as 0
    target_position = 0;
}

void motorControl_run() {
    // Simple bang-bang controller with +/- 3mm deadband
    uint16_t current_position = actuator_get_position();

    if(current_position > (target_position + 6)) {
        actuator_set_power(400, false);
    } else if (current_position < (target_position - 6)) {
        actuator_set_power(400, true);
    } else {
        actuator_set_power(0, true);
    }
}

void motorControl_setTarget(uint16_t target) {
    target_position = target;
}

uint16_t motorControl_getTarget() {
    return target_position;
}