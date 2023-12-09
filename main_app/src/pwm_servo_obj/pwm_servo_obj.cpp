#include "pwm_servo_obj.h"

// #include "Servo.h"
#include <cmath>

Servo::Servo(PCA9685* pca, uint8_t pin, uint16_t minPulseWidth, uint16_t maxPulseWidth)
:   pca_(pca),
    pin_(pin),
    minPulseWidth_(minPulseWidth),
    maxPulseWidth_(maxPulseWidth)
{
    // unitDuration_ = pca_->calcUnitDurationUs();
    pca_->setFreq_Hz(100);
    deactivate();
    pca_->wakeUp();
}

void Servo::_setAngle(float angle) {
    float pulseWidth = map(angle, 0.0f, 180.0f, minPulseWidth_, maxPulseWidth_);
    // uint16_t onTime = static_cast<uint16_t>(round(pulseWidth * unitDuration_));
    pca_->setDutyCycle(pin_, 0, pulseWidth);
}

void Servo::setAngleDegrees(float angle) {
    _setAngle(angle); // angle уже в градусах
}

void Servo::setAngleRadians(float angle) {
    float angleDegrees = angle * (180.0f / M_PI); // Преобразование из радиан в градусы
    _setAngle(angleDegrees);
}

void Servo::deactivate() {
    pca_->setDutyCycle(pin_, 0, 0);
}

float Servo::map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}