#include "PS_MG996R.hpp"

MG996R::MG996R(
    ProtoPWM& pwm,
    MicroSeconds minPulse,
    MicroSeconds maxPulse,
    Angle maxAngle,
    Angle offset,
    rotDir_t reversed
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

#include "terminal_tools.h"
void MG996R::setAngle(const Angle& angle) {
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

bool  MG996R::isEnabled()       const { return _enabled; }
Angle MG996R::getAngle()        const { return _currentAngle; }
Angle MG996R::getAngleOffset()  const { return _offset;}
void  MG996R::setAngleOffset(const Angle& angle)        { _offset = angle; }
void  MG996R::setReversion(const rotDir_t isReversed )  { _reversed = isReversed; }

void  MG996R::enable() {
    _enabled = true;
    setAngle(_currentAngle);
}

void  MG996R::disable() {
    _enabled = false;
    _pwm.setDuration(us(0)); //TODO: make real disabled
}
