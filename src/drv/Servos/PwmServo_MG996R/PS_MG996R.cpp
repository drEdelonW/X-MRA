#include "PS_MG996R.hpp"

MG996R::MG996R(
    ProtoPWM& pwm,
    MicroSeconds minPulse,
    MicroSeconds maxPulse,
    Angle maxAngle,
    Angle offset,
    bool reversed)
    : _pwm(pwm),
      _minPulse(minPulse),
      _maxPulse(maxPulse),
      _maxAngle(maxAngle),
      _offset(offset),
      _reversed(reversed),
      _currentAngle(rad(0.0f)),
      _enabled(true) {}

#include "terminal_tools.h"

void MG996R::setAngle(const Angle& angle) {
    if(!_enabled)
        return;
    if (isnan(angle.asRadians()))
        return;
    _currentAngle = angle;

    Angle corrected = angle + _offset;
    if (_reversed) {
        corrected = _maxAngle - corrected;
    }

    float normalized = clamp(corrected.asRadians() / _maxAngle.asRadians(), 0.0f, 1.0f);

    _pwm.setDuration(
        _minPulse + ((_maxPulse - _minPulse) * normalized)
    );
    _currentAngle = angle;
}

Angle MG996R::getAngle() const { return _currentAngle; }

void  MG996R::setAngleOffset(const Angle& angle) { _offset = angle; }
Angle MG996R::getAngleOffset() const { return _offset;}

void MG996R::setReversion(const bool isReversed ){_reversed = isReversed; }

bool MG996R::isEnabled() const { return _enabled; }

void MG996R::enable() {
    _enabled = true;
    setAngle(_currentAngle);
}

void MG996R::disable() {
    _enabled = false;
    _pwm.setDuration(0);
}
