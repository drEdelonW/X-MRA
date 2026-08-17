#pragma once
#include "proto_pwmServo.hpp"   // ProtoServo
#include "proto_PWM.hpp"        // ProtoPWM

// #define FreeNove_noname_cfg     deg(180.f), us(560), us(2650)
class sFreeNove : public ProtoServo {
public:
    sFreeNove(
        ProtoPWM& pwm,
        rotDir_t reversed       = CW,
        Angle offset            = deg(0.f),
        Angle maxAngle          = deg(180.f),
        MicroSeconds minPulse   = us(560),
        MicroSeconds maxPulse   = us(2650)
    );

    Angle getAngle() const override;
    void  setAngle(const Angle& angle) override;

    Angle getAngleOffset() const override;
    void  setAngleOffset(const Angle& angle) override;

    Angle getAngleMax() const override;

    void setReversion(const rotDir_t isReversed) override;

    void enable() override;
    void disable() override;
    bool isEnabled() const override;

private:
    ProtoPWM& _pwm;

    MicroSeconds  _minPulse;
    MicroSeconds  _PulseRange;

    Angle   _maxAngle;
    Angle   _offset;

    rotDir_t _reversed;
    Angle   _currentAngle;
    bool    _enabled;
};