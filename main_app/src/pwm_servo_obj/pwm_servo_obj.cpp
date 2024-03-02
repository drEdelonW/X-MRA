#include "pwm_servo_obj.h"

#include <cmath>
#include <limits>

Servo::Servo(PCA9685* pca, uint8_t pin, uint16_t minPulseWidth, uint16_t maxPulseWidth, float maxAngle)
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
    return currentAngleDegrees_ * (M_PI / 180.0f); // Convert angle to radians
}
void Servo::setAngleRadians(float angle) {
    float angleDegrees = angle * (180.0f / M_PI); // Convert from radians to degrees
    _setAngle(angleDegrees);
}

uint16_t Servo::getMinPulseWidth() const {
    return minPulseWidth_;
}
void Servo::setMinPulseWidth(uint16_t minPulseWidth) {
    minPulseWidth_ = minPulseWidth;
}

uint16_t Servo::getMaxPulseWidth() const {
    return maxPulseWidth_;
}
void Servo::setMaxPulseWidth(uint16_t maxPulseWidth) {
    maxPulseWidth_ = maxPulseWidth;
}

uint16_t Servo::getPhaseShift() const {
    return phaseShift_;
}
void Servo::setPhaseShift(uint16_t phaseShift) {
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
    uint16_t duration = std::min(static_cast<uint16_t>(pulseWidth), maxValue_);
    pca_->setDutyCycle(pin_, duration, phaseShift_);
}

float Servo::_map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}