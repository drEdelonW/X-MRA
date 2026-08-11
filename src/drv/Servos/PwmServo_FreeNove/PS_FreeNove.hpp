#pragma once
#include "proto_pwmServo.hpp"
#include "proto_PWM.hpp"

#define FreeNove_noname_cfg     us(560), us(2650), deg(160), deg(90)
class sFreeNove : public ProtoServo {
public:
    sFreeNove(
        ProtoPWM& pwm,
        rotDir_t reversed       = CW,
        MicroSeconds minPulse   = us(560),
        MicroSeconds maxPulse   = us(2650),
        Angle maxAngle          = deg(160),
        Angle offset            = deg(90)
    );

    void  setAngle(const Angle& angle) override;
    Angle getAngle() const override;

    void  setAngleOffset(const Angle& angle) override;
    Angle getAngleOffset() const override;

    Angle getAngleMax() const override;

    void setReversion(const rotDir_t isReversed) override;

    void enable() override;
    void disable() override;
    bool isEnabled() const override;

private:
    ProtoPWM& _pwm;

    MicroSeconds    _minPulse;
    MicroSeconds    _maxPulse;
    Angle   _maxAngle;
    Angle   _offset;

    rotDir_t _reversed;
    Angle   _currentAngle;
    bool    _enabled;
};