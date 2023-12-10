#include "pwm_servo_obj.h"

// #include "Servo.h"
#include <cmath>

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

void Servo::_setAngle(float angle) {
    currentAngleDegrees_ = angle; // Кешируем угол в градусах
    float pulseWidth = _map(angle, 0.0f, maxAngle_, minPulseWidth_, maxPulseWidth_);
    uint16_t duration = std::min(static_cast<uint16_t>(pulseWidth), maxValue_);
    pca_->setDutyCycle(pin_, duration, phaseShift_);
}

void Servo::setAngleDegrees(float angle) {
    _setAngle(angle); // angle уже в градусах
}

void Servo::setAngleRadians(float angle) {
    float angleDegrees = angle * (180.0f / M_PI); // Преобразование из радиан в градусы
    _setAngle(angleDegrees);
}

void Servo::setMinPulseWidth(uint16_t minPulseWidth) {
    minPulseWidth_ = minPulseWidth;
}

void Servo::setMaxPulseWidth(uint16_t maxPulseWidth) {
    maxPulseWidth_ = maxPulseWidth;
}

void Servo::setMaxAngle(float maxAngle) {
    maxAngle_ = maxAngle;
}

void Servo::setPhaseShift(uint16_t phaseShift) {
    phaseShift_ = phaseShift;
}

float Servo::getAngleDegrees() const {
    return currentAngleDegrees_;
}

float Servo::getAngleRadians() const {
    return currentAngleDegrees_ * (M_PI / 180.0f); // Преобразование угла в радианы
}

uint16_t Servo::getMinPulseWidth() const {
    return minPulseWidth_;
}

uint16_t Servo::getMaxPulseWidth() const {
    return maxPulseWidth_;
}

float Servo::getMaxAngle() const {
    return maxAngle_;
}

uint16_t Servo::getPhaseShift() const {
    return phaseShift_;
}

void Servo::deactivate() {
    pca_->setDutyCycle(pin_, 0, 0);
    currentAngleDegrees_ = std::numeric_limits<float>::quiet_NaN(); // Устанавливаем угол как NaN
}

float Servo::_map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}