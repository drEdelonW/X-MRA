#include "ArachnidLeg.hpp"

#include <cmath>

ArachnidLeg::ArachnidLeg(Servo* coxaServo, Servo* femurServo, Servo* tibiaServo)
: coxaServo_(coxaServo), femurServo_(femurServo), tibiaServo_(tibiaServo) {}

void ArachnidLeg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
    coxaServo_->setAngleDegrees(coxaAngle);
    femurServo_->setAngleDegrees(femurAngle);
    tibiaServo_->setAngleDegrees(tibiaAngle);
}

void ArachnidLeg::deactivate() {
    coxaServo_->deactivate();
    femurServo_->deactivate();
    tibiaServo_->deactivate();
}

void ArachnidLeg::setTipPosition(float x, float y, float z) {
    float distanceToTarget = sqrt(x * x + y * y);

    // Вычисляем углы для фемура и тибии с использованием обратной кинематики
    float angleFemur = acos((tibiaLength_*tibiaLength_ - femurLength_*femurLength_ - distanceToTarget*distanceToTarget) / (-2 * femurLength_ * distanceToTarget));
    float angleTibia = acos((distanceToTarget*distanceToTarget - femurLength_*femurLength_ - tibiaLength_*tibiaLength_) / (-2 * femurLength_ * tibiaLength_));

    // Вычисляем угол для коксы
    float angleCoxa = atan2(y, x);

    // Установка углов суставов напрямую в радианах
    coxaServo_->setAngleRadians(angleCoxa);
    femurServo_->setAngleRadians(angleFemur);
    tibiaServo_->setAngleRadians(angleTibia);
}
std::tuple<float, float, float> ArachnidLeg::getTipPosition() const {
    // Реализация получения текущего положения кончика ноги
    // ...
    return std::make_tuple(0.0f, 0.0f, 0.0f);
}
