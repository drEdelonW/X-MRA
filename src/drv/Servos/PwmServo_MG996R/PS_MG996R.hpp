#pragma once
#include "proto_pwmServo.hpp"
#include "proto_PWM.hpp"

class MG996R : public ProtoServo {
public:
    MG996R(ProtoPWM& pwm,
             MicroSeconds minPulse = 50,
             MicroSeconds maxPulse = 150,
             Angle maxAngle = deg(180.0f),
             Angle offset = deg(0.0f),
             bool reversed = false);

    void setAngle(const Angle& angle) override;
    Angle getAngle() const override;

    void enable() override;
    void disable() override;
    bool isEnabled() const override;

private:
    ProtoPWM& pwm_;
    MicroSeconds minPulse_;
    MicroSeconds maxPulse_;
    Angle maxAngle_;
    Angle offset_;
    bool reversed_;
    Angle currentAngle_;
};