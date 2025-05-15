#pragma once
#include "proto_pwmServo.hpp"
#include "proto_PWM.hpp"

#define TowerPro_MG996R_cfg     400, 2600, deg(210), deg(102)
#define TowerPro_MG90S_cfg      650, 2400, deg(185), deg(92)

#define HailangNiao_MG996R_cfg  525, 2500, deg(220), deg(115)

#define CW  true
#define CCW false

class MG996R : public ProtoServo {
public:
    MG996R(
        ProtoPWM& pwm,
        MicroSeconds minPulse   = 600,
        MicroSeconds maxPulse   = 2500,
        Angle maxAngle          = deg(180.0f),
        Angle offset            = deg(90.0f),
        bool reversed           = CW
    );

    void  setAngle(const Angle& angle) override;
    Angle getAngle() const override;

    void  setAngleOffset(const Angle& angle) override;
    Angle getAngleOffset() const override;

    void setReversion(const bool isReversed) override;

    void enable() override;
    void disable() override;
    bool isEnabled() const override;

private:
    ProtoPWM& _pwm;

    MicroSeconds _minPulse;
    MicroSeconds _maxPulse;
    Angle _maxAngle;
    Angle _offset;

    bool  _reversed;
    Angle _currentAngle;
    bool  _enabled;
};