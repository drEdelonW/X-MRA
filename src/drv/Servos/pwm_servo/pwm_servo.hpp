#pragma once
#include "proto_PWM.hpp"


    #include "PA_PCA9685.hpp"

#define PWM_UNIT    uint16_t

class Servo {
public:
    // Constructor: initializes a servo on a specific PWM_ARRAY channel with min and max pulse widths
    Servo(PWM_ARRAY* pca, uint8_t pin, PWM_UNIT minPulseWidth = 180, PWM_UNIT maxPulseWidth = 1040, float maxAngle = 180.0f);

    float    getAngleDegrees() const;
    void     setAngleDegrees(float angle); // Set the servo angle in degrees

    float    getAngleRadians() const;
    void     setAngleRadians(float angle); // Set the servo angle in radians

    PWM_UNIT getMinPulseWidth() const;
    void     setMinPulseWidth(PWM_UNIT minPulseWidth);

    PWM_UNIT getMaxPulseWidth() const;
    void     setMaxPulseWidth(PWM_UNIT maxPulseWidth);

    PWM_UNIT getPhaseShift() const;
    void     setPhaseShift(PWM_UNIT phaseShift);

    void     setMaxAngle(float maxAngle);
    float    getMaxAngle() const;

    void deactivate(); // Deactivate (turn off) the servo signal

private:
    PWM_ARRAY*    pca_;  // Pointer to PWM_ARRAY controller
    uint8_t     pin_;   // Servo pin on the PWM_ARRAY

    PWM_UNIT    minPulseWidth_;    // Min pulse width for 0-degree position
    PWM_UNIT    maxPulseWidth_;    // Max pulse width for 180-degree position

    float       maxAngle_;
    PWM_UNIT    phaseShift_; // Current phase shift for the servo

    PWM_UNIT    maxValue_; // Maximum PWM value obtained from PWM_ARRAY

    float       currentAngleDegrees_; // Current servo angle in degrees

    void    _setAngle(float angle);    // Set the servo angle in degrees
    float   _map(float x, float in_min, float in_max, float out_min, float out_max);  // Map a value from one range to another
};
