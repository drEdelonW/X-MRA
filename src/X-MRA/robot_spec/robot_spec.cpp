// #include "pca9685_obj.h"
// #include "pwm_servo_obj.h"
#include <stdio.h>
#include <unistd.h>
#include "robot_spec.hpp"


PWM_ARRAY PWM[2] = {
    PWM_ARRAY(1, 0x40),
    PWM_ARRAY(1, 0x41)
};

Servo joint[9] = {
    Servo(&PWM[1], 2),
    Servo(&PWM[0], 1),
    Servo(&PWM[0], 0),

    Servo(&PWM[0], 6),
    Servo(&PWM[0], 5),
    Servo(&PWM[0], 4),

    Servo(&PWM[1], 13),
    Servo(&PWM[1], 14),
    Servo(&PWM[1], 15)
};

ArachnidLeg BR = ArachnidLeg(
        &joint[0],
        &joint[1],
        &joint[2]
    );

ArachnidLeg CR = ArachnidLeg(
        &joint[3],
        &joint[4],
        &joint[5]
    );

ArachnidLeg FR = ArachnidLeg(
        &joint[6],
        &joint[7],
        &joint[8]
    );


void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us) {
    PWM[0].setFreq_Hz(freq);
    printf("new freq %f\n", PWM[0].getFreq_Hz());
    PWM[0].wakeUp();
    for(int i = min_dur; i < max_dur; i+=1){
        usleep(delay_us);
        PWM[0].setDutyCycle(0, 0, i);
        // PWM[0].setDutyCycle(1, 0, i);
    }
    PWM[0].setDutyCycle(0, 0, (min_dur + max_dur) / 2);
    printf("mid is %d\n", (min_dur + max_dur) / 2);
}
#define ANGLE_MID   (90.0f)

void testLegs() {
    joint[0].setPhaseShift(3500);
    PWM[0].wakeUp();
    PWM[1].wakeUp();
    printf("first\n");
    BR.setJointAngles( 15.0, 15.0, ANGLE_MID); usleep(1000 * 2000);
    printf("second\n");
    BR.setJointAngles( 90.0, 90.0, ANGLE_MID); usleep(1000 * 2000);
    printf("third\n");
    BR.setJointAngles(170.0, 170.0, ANGLE_MID); usleep(1000 * 2000);

    // CR.setJointAngles( 55.0, 189.0f, 180.0f); //usleep(1000 * 2000);
    // BR.setJointAngles( 15.0, 185.0f, 165.0f); usleep(1000 * 5000);
    CR.deactivate();
    BR.deactivate();

        // CR.setJointAngles(5.0, 5.0, 5.0);
    // FR.setJointAngles(5.0, 5.0, 5.0);
    // usleep(1000 * 3000);
    // BR.setJointAngles(5.0, 5.0, 5.2);
    // CR.setJointAngles(5.0, 5.0, 5.2);
    // FR.setJointAngles(5.0, 5.0, 5.2);
    // usleep(1000 * 3000);
    // BR.setJointAngles(5.0, 5.0, 5.2);
    // CR.setJointAngles(5.0, 5.0, 5.2);
    // FR.setJointAngles(5.0, 5.0, 5.2);
    // usleep(1000 * 10000);

    PWM[0].sleepMode();
    PWM[1].sleepMode();
}
