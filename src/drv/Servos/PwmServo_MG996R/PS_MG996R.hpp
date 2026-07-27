#pragma once
#include "proto_pwmServo.hpp"
#include "proto_PWM.hpp"

#define TowerPro_MG996R_cfg     us(400), us(2600), deg(210), deg(102)
#define TowerPro_MG90S_cfg      us(650), us(2400), deg(185), deg(92)
#define HailangNiao_MG996R_cfg  us(525), us(2500), deg(220), deg(115)
#define FreeNove_noname_cfg     us(560), us(2650), deg(160), deg(90)

class MG996R : public ProtoServo {
public:
    MG996R(
        ProtoPWM& pwm,
        MicroSeconds minPulse   = us(650),
        MicroSeconds maxPulse   = us(2400),
        Angle maxAngle          = deg(180.0f),
        Angle offset            = deg(90.0f),
        rotDir_t reversed       = CW
    );

    void  setAngle(const Angle& angle) override;
    Angle getAngle() const override;

    void  setAngleOffset(const Angle& angle) override;
    Angle getAngleOffset() const override;

    void setReversion(const rotDir_t isReversed) override;

    void enable() override;
    void disable() override;
    bool isEnabled() const override;

private:
    ProtoPWM& _pwm;

    MicroSeconds _minPulse;
    MicroSeconds _maxPulse;
    Angle _maxAngle;
    Angle _offset;

    rotDir_t _reversed;
    Angle _currentAngle;
    bool  _enabled;
};