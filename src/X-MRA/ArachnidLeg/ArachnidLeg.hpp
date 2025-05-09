#pragma once
#include "pwm_servo.hpp"
#include <tuple>

class ArachnidLeg {
public:
    ArachnidLeg(Servo* coxaServo, Servo* femurServo, Servo* tibiaServo);

    void setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);   // Set joint angles directly

    void setTipPosition(float x, float y, float z);             // Set leg tip position using inverse kinematics
    std::tuple<float, float, float> getTipPosition() const;     // Get current tip position (forward kinematics or cached)

    void deactivate();

private:
    Servo* coxaServo_;
    Servo* femurServo_;
    Servo* tibiaServo_;

    const Millimeters coxaLength_ = 4.0f;    // Length of coxa segment
    const Millimeters femurLength_ = 3.5f;   // Length of femur segment
    const Millimeters tibiaLength_ = 8.7f;   // Length of tibia segment

    // Inverse kinematics calculation
    void calculateInverseKinematics(float x, float y, float z);
};
