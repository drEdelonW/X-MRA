#pragma once 
#include "pca9685_obj.h"

class Servo {
public:
    // Constructor: initializes a servo on a specific PCA9685 channel with min and max pulse widths
    Servo(PCA9685* pca, uint8_t pin, uint16_t minPulseWidth = 180, uint16_t maxPulseWidth = 1040, float maxAngle = 180.0f);

    void setAngleDegrees(float angle); // Set the servo angle in degrees
    void setAngleRadians(float angle); // Set the servo angle in radians
    void setMinPulseWidth(uint16_t minPulseWidth);
    void setMaxPulseWidth(uint16_t maxPulseWidth);
    void setMaxAngle(float maxAngle);
    void setPhaseShift(uint16_t phaseShift);

    float getAngleDegrees() const;
    float getAngleRadians() const;
    uint16_t getMinPulseWidth() const;
    uint16_t getMaxPulseWidth() const;
    float getMaxAngle() const;
    uint16_t getPhaseShift() const;

    void deactivate(); // Deactivate (turn off) the servo signal

private:
    PCA9685* pca_;  // Pointer to PCA9685 controller
    uint8_t pin_;   // Servo pin on the PCA9685
    uint16_t minPulseWidth_;    // Min pulse width for 0-degree position
    uint16_t maxPulseWidth_;    // Max pulse width for 180-degree position
    float maxAngle_; 
    uint16_t phaseShift_; // Текущий фазовый сдвиг для сервопривода

    uint16_t maxValue_; // Максимальное значение ШИМ, полученное от PCA9685

    float currentAngleDegrees_; // Текущий угол сервопривода в градусах

    void _setAngle(float angle);    // Set the servo angle in degrees
    
    float _map(float x, float in_min, float in_max, float out_min, float out_max);  // Map a value from one range to another
};
