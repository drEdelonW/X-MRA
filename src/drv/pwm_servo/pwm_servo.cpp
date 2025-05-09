#include "pwm_servo.hpp"
#include "MachTypes.hpp"

#include <array>

#include <cmath>
#include <limits>

Servo::Servo(PWM_ARRAY* pca, uint8_t pin, PWM_UNIT minPulseWidth, PWM_UNIT maxPulseWidth, float maxAngle)
:   pca_(pca),
    pin_(pin),
    minPulseWidth_(minPulseWidth),
    maxPulseWidth_(maxPulseWidth),
    maxAngle_(maxAngle),
    maxValue_(pca_->getMaxValue())
{
    pca_->setFreq_Hz(100);
    deactivate();
}

float Servo::getAngleDegrees() const {
    return currentAngleDegrees_;
}
void Servo::setAngleDegrees(float angle) {
    _setAngle(angle); // angle is already in degrees
}

float Servo::getAngleRadians() const {
    return DEG_TO_RAD(currentAngleDegrees_); // Convert angle to radians
}
void Servo::setAngleRadians(float angle) {
    float angleDegrees = DEG_TO_RAD(angle); // Convert from radians to degrees
    _setAngle(angleDegrees);
}

PWM_UNIT Servo::getMinPulseWidth() const {
    return minPulseWidth_;
}
void Servo::setMinPulseWidth(PWM_UNIT minPulseWidth) {
    minPulseWidth_ = minPulseWidth;
}

PWM_UNIT Servo::getMaxPulseWidth() const {
    return maxPulseWidth_;
}
void Servo::setMaxPulseWidth(PWM_UNIT maxPulseWidth) {
    maxPulseWidth_ = maxPulseWidth;
}

PWM_UNIT Servo::getPhaseShift() const {
    return phaseShift_;
}
void Servo::setPhaseShift(PWM_UNIT phaseShift) {
    phaseShift_ = phaseShift;
}

void Servo::setMaxAngle(float maxAngle) {
    maxAngle_ = maxAngle;
}
float Servo::getMaxAngle() const {
    return maxAngle_;
}

void Servo::deactivate() {
    pca_->setDutyCycle(pin_, 0, 0);
    currentAngleDegrees_ = std::numeric_limits<float>::quiet_NaN(); // Set angle as NaN
}


void Servo::_setAngle(float angle) {
    currentAngleDegrees_ = angle; // Cache the angle in degrees
    float pulseWidth = _map(
        angle,
        0.0f,           maxAngle_,
        minPulseWidth_, maxPulseWidth_
    );
    PWM_UNIT duration = std::min(static_cast<PWM_UNIT>(pulseWidth), maxValue_);
    pca_->setDutyCycle(pin_, duration, phaseShift_);
}

float Servo::_map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}