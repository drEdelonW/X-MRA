#pragma once 
#include "pwm_servo_obj.h"
#include <tuple>

class HexapodLeg {
public:
    // Конструктор
    HexapodLeg(Servo* coxaServo, Servo* femurServo, Servo* tibiaServo);

    // Установка положения суставов ноги
    void setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);

    // Установка положения кончика ноги с помощью обратной кинематики
    void setTipPosition(float x, float y, float z);

    // Получение текущего положения кончика ноги
    std::tuple<float, float, float> getTipPosition() const;

private:
    Servo* coxaServo_;  // Сервопривод сустава коксы
    Servo* femurServo_; // Сервопривод сустава фемура
    Servo* tibiaServo_; // Сервопривод сустава тибии

    // Длины сегментов
    const float coxaLength_ = 4.0f;    // Длина коксы
    const float femurLength_ = 3.5f;   // Длина фемура
    const float tibiaLength_ = 8.7f;   // Длина тибии

    // Обратная кинематика для расчета положения кончика ноги
    void calculateInverseKinematics(float x, float y, float z);
};