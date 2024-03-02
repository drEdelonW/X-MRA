#pragma once 
#include "pca9685_obj.h"

class Servo {
public:
    // Constructor: initializes a servo on a specific PCA9685 channel with min and max pulse widths
    Servo(PCA9685* pca, uint8_t pin, uint16_t minPulseWidth = 180, uint16_t maxPulseWidth = 1040, float maxAngle = 180.0f);

    float    getAngleDegrees() const;
    void     setAngleDegrees(float angle); // Set the servo angle in degrees

    float    getAngleRadians() const;
    void     setAngleRadians(float angle); // Set the servo angle in radians

    uint16_t getMinPulseWidth() const;
    void     setMinPulseWidth(uint16_t minPulseWidth);

    uint16_t getMaxPulseWidth() const;
    void     setMaxPulseWidth(uint16_t maxPulseWidth);

    uint16_t getPhaseShift() const;
    void     setPhaseShift(uint16_t phaseShift);

    void     setMaxAngle(float maxAngle);
    float    getMaxAngle() const;

    void deactivate(); // Deactivate (turn off) the servo signal

private:
    PCA9685*    pca_;  // Pointer to PCA9685 controller
    uint8_t     pin_;   // Servo pin on the PCA9685

    uint16_t    minPulseWidth_;    // Min pulse width for 0-degree position
    uint16_t    maxPulseWidth_;    // Max pulse width for 180-degree position
    
    float       maxAngle_; 
    uint16_t    phaseShift_; // Current phase shift for the servo

    uint16_t    maxValue_; // Maximum PWM value obtained from PCA9685

    float       currentAngleDegrees_; // Current servo angle in degrees

    void    _setAngle(float angle);    // Set the servo angle in degrees
    float   _map(float x, float in_min, float in_max, float out_min, float out_max);  // Map a value from one range to another
};
