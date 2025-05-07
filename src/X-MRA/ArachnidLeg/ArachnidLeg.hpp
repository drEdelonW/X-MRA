#pragma once 
#include "pwm_servo.hpp"
#include <tuple>

class ArachnidLeg {
public:
    // Конструктор
    ArachnidLeg(Servo* coxaServo, Servo* femurServo, Servo* tibiaServo);

    // Установка положения суставов ноги
    void setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);

    // Установка положения кончика ноги с помощью обратной кинематики
    void setTipPosition(float x, float y, float z);

    void deactivate();
    // Получение текущего положения кончика ноги
    std::tuple<float, float, float> getTipPosition() const;

private:
    Servo* coxaServo_;
    Servo* femurServo_;
    Servo* tibiaServo_;

    const float coxaLength_ = 4.0f;    // Длина коксы
    const float femurLength_ = 3.5f;   // Длина фемура
    const float tibiaLength_ = 8.7f;   // Длина тибии

    // Обратная кинематика для расчета положения кончика ноги
    void calculateInverseKinematics(float x, float y, float z);
};