#pragma once

#include "ArachnidLeg.hpp"

#include "PA_PCA9685.hpp"
#include "PS_MG996R.hpp"
#include "pwmServoJoint.hpp"

extern PCA9685 PWMarray;
extern MG996R Servo[];


void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us);

void testLegs();
