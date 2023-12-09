#pragma once 
#include "pca9685_obj.h"

class Servo {
public:
    // Constructor: initializes a servo on a specific PCA9685 channel with min and max pulse widths
    Servo(PCA9685* pca, uint8_t pin, uint16_t minPulseWidth, uint16_t maxPulseWidth);
    
    void setAngleDegrees(float angle); // Set the servo angle in degrees
    void setAngleRadians(float angle); // Set the servo angle in radians

    // Deactivate (turn off) the servo signal
    void deactivate();

private:
    PCA9685* pca_; // Pointer to PCA9685 controller
    uint8_t pin_; // Servo pin on the PCA9685
    int unitDuration_; 
    uint16_t minPulseWidth_; // Min pulse width for 0-degree position
    uint16_t maxPulseWidth_; // Max pulse width for 180-degree position

    void _setAngle(float angle); // Set the servo angle in degrees
    // Map a value from one range to another
    float map(float x, float in_min, float in_max, float out_min, float out_max);
};
