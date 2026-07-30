#pragma once

#include "JointBase.hpp"
#include "proto_pwmServo.hpp"

class ServoJoint : public JointBase {
public:
    ServoJoint(
        ProtoServo& servo,
        Angle minLim = deg(-90.f),
        Angle maxLim = deg( 90.f),
        Angle offset = deg(  0.f)
    ):
        _servo(servo),
        _min(minLim),
        _max(maxLim),
        _offset(offset),
        _plan(deg(NAN)),
        _valid(false),
        _skip(false)
    {}

    bool checkPose(const Angle& logical) override {
        if (std::isnan(logical.asRadians()))  // NaN -> skip on apply
            return _skip = true;

        _skip = false;

        Angle phys = logical + _offset;
        if ((phys < _min) ||
            (phys > _max)
        )   return _valid = false;

        _plan = phys;

        return _valid = true;
    }

    bool applyPose() override {
        if (_skip)      // NaN -> skip on apply
            return true;

        if (!_valid)
            return false;

        _servo.setAngle(_plan);

        return true;
    }

    Angle getAngle() const override { return _servo.getAngle() - _offset; }

    void engage() override  { _servo.enable(); }
    void release() override { _servo.disable(); }

private:
    ProtoServo& _servo;

    Angle _min, _max;
    Angle _offset;
    Angle _plan;
    bool  _valid;
    bool  _skip;
};
