#pragma once
#include "proto_pwmServo.hpp"
#include "PhysTypes.hpp"
// #include <tuple>

class ArachnidLeg {
public:
    ArachnidLeg(ProtoServo& coxaServo, ProtoServo& femurServo, ProtoServo& tibiaServo);

    void setJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle);   // Set joint angles directly

    void setTipPosition(float x, float y, float z);             // Set leg tip position using inverse kinematics
    // std::tuple<float, float, float> getTipPosition() const;     // Get current tip position (forward kinematics or cached)

    void deactivate();

private:
ProtoServo& coxaServo_;
ProtoServo& femurServo_;
ProtoServo& tibiaServo_;

    const Millimeters coxaLength_ = 4.0f;    // Length of coxa segment
    const Millimeters femurLength_ = 3.5f;   // Length of femur segment
    const Millimeters tibiaLength_ = 8.7f;   // Length of tibia segment

    // Inverse kinematics calculation
    void calculateInverseKinematics(Millimeters x, Millimeters y, Millimeters z);
};
