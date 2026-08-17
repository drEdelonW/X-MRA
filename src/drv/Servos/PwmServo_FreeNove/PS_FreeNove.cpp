#include "PS_FreeNove.hpp"

sFreeNove::sFreeNove(
    ProtoPWM& pwm,
    rotDir_t reversed,
    Angle offset,
    Angle maxAngle,
    MicroSeconds minPulse,
    MicroSeconds maxPulse
):
    _pwm(pwm),
    _minPulse(minPulse),
    _PulseRange(maxPulse - _minPulse),
    _maxAngle(maxAngle),
    _offset(offset),
    _reversed(reversed),
    _currentAngle(rad(0.f)),
    _enabled(true)
{}

#include "CLAMP.h"
Angle sFreeNove::getAngle() const { return _currentAngle; }
void  sFreeNove::setAngle(const Angle& angle) {
    if ((!_enabled) ||
        (isnan(angle.asRadians()))
    )   return;

    Angle hwAng = (angle + _offset);
    Angle corrected = (_reversed == CCW)?
        hwAng : (-hwAng + _maxAngle);

    float rangeFactor =  corrected.asRAW() / _maxAngle.asRAW();
    ClampInRange(0.f, &rangeFactor, 1.f);
    _pwm.setDuration(
        _minPulse +
        (_PulseRange * rangeFactor)
    );
    _currentAngle = angle;
}

Angle sFreeNove::getAngleOffset()   const { return _offset;}
void  sFreeNove::setAngleOffset(const Angle& angle)     { _offset = angle; }

Angle sFreeNove::getAngleMax()  const { return _maxAngle;}
void  sFreeNove::setReversion(const rotDir_t isReversed)    { _reversed = isReversed; }

bool  sFreeNove::isEnabled()    const { return _enabled; }
void  sFreeNove::enable() {
    _enabled = true;
    setAngle(_currentAngle);
}

void  sFreeNove::disable() {
    _enabled = false;
    _pwm.setDuration(us(0)); //TODO: make real disabled
}
