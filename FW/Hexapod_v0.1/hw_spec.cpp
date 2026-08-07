#include "hw_spec.h"

void leg_init() {
    for (int i = 0; i < LEG_NUM; i++)  {
        legs[i].SCOXA.attach( legs[i].COXA,  SERVO_MIN, SERVO_MAX);
        legs[i].SFEMUR.attach(legs[i].FEMUR, SERVO_MIN, SERVO_MAX);
        legs[i].STIBIA.attach(legs[i].TIBIA, SERVO_MIN, SERVO_MAX);
    }
}