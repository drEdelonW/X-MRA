#pragma once

// #include "pca9685_obj.h"
// #include "pwm_servo_obj.h"
#include "hexapod_leg_obj.h"

// PCA9685 PWM[2];

// Servo servos[9];

// HexapodLeg BR;
// HexapodLeg CR;
// HexapodLeg FR;

void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us);

void testLegs();
