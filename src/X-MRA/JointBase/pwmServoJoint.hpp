#pragma once

#include "JointBase.hpp"
#include "proto_pwmServo.hpp"

class ServoJoint : public JointBase {
public:
    ServoJoint(ProtoServo& servo) : _servo(servo) {}

    void  setAngle(const Angle& angle) override     { _servo.setAngle(angle); }
    Angle getAngle() const override                 { return _servo.getAngle(); }

    void engage() override  { _servo.enable(); }
    void release() override { _servo.disable(); }

private:
    ProtoServo& _servo;
};
