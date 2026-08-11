#pragma once
#include "proto_pwmServo.hpp"   // ProtoServo
#include "proto_PWM.hpp"        // ProtoPWM

#define TowerPro_MG996R_cfg     us(400), us(2600), deg(210), deg(102)
#define TowerPro_MG90S_cfg      us(650), us(2400), deg(185), deg(92)
#define HailangNiao_MG996R_cfg  us(525), us(2500), deg(220), deg(115)


class MG996R : public ProtoServo {
public:
    MG996R(
        ProtoPWM& pwm,
        rotDir_t reversed       = CW,
        MicroSeconds minPulse   = us(650),  // TowerPro_MG90S_cfg
        MicroSeconds maxPulse   = us(2400),
        Angle maxAngle          = deg(185),
        Angle offset            = deg(92)
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

    MicroSeconds    _minPulse;
    MicroSeconds    _maxPulse;
    Angle   _maxAngle;
    Angle   _offset;

    rotDir_t _reversed;
    Angle   _currentAngle;
    bool    _enabled;
};