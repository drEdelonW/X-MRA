#include "PS_MG996R.hpp"

MG996R::MG996R(
    ProtoPWM& pwm,
    MicroSeconds minPulse,
    MicroSeconds maxPulse,
    Angle maxAngle,
    Angle offset,
    bool reversed)
    : pwm_(pwm),
      minPulse_(minPulse),
      maxPulse_(maxPulse),
      maxAngle_(maxAngle),
      offset_(offset),
      reversed_(reversed),
      currentAngle_(Angle::fromRadians(0.0f)) {}

void MG996R::setAngle(const Angle& angle) {
    Angle corrected = angle + offset_;
    if (reversed_) {
        corrected = maxAngle_ - corrected;
    }

    float normalized = clamp(corrected.asRadians() / maxAngle_.asRadians(), 0.0f, 1.0f);
    MicroSeconds duration = static_cast<MicroSeconds>(
        minPulse_ + normalized * (maxPulse_ - minPulse_)
    );

    pwm_.setPulseDurationUS(duration);
    currentAngle_ = angle;
}

Angle MG996R::getAngle() const {
    return currentAngle_;
}

void MG996R::enable() {
    pwm_.enable();
}

void MG996R::disable() {
    pwm_.disable();
}

bool MG996R::isEnabled() const {
    return pwm_.isEnabled();
}
