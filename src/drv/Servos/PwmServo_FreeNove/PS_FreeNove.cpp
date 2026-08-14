#include "PS_FreeNove.hpp"

sFreeNove::sFreeNove(
    ProtoPWM& pwm,
    rotDir_t reversed,
    MicroSeconds minPulse,
    MicroSeconds maxPulse,
    Angle maxAngle,
    Angle offset
):
    _pwm(pwm),
    _minPulse(minPulse),
    _maxPulse(maxPulse),
    _maxAngle(maxAngle),
    _offset(offset),
    _reversed(reversed),
    _currentAngle(rad(0.f)),
    _enabled(true)
{}

Angle sFreeNove::getAngle() const { return _currentAngle; }
void  sFreeNove::setAngle(const Angle& angle) {
    if ((!_enabled) ||
        (isnan(angle.asRadians()))
    )   return;

    _currentAngle = angle;
    Angle hwAng = (angle + _offset);
    Angle corrected = (!_reversed)?
        hwAng : (-hwAng + _maxAngle);

    _pwm.setDuration(
        _minPulse + (
            (_maxPulse - _minPulse) *
            Clamp(
                0.f,
                corrected.asRadians() /
                _maxAngle.asRadians(),
                1.f
            )
        )
    );
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
