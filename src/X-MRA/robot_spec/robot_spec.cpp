// #include "pca9685_obj.h"
// #include "pwm_servo_obj.h"
#include <stdio.h>
#include <unistd.h>
#include "robot_spec.hpp"


PCA9685 PWMarray[] = {
    {1, 0x40},
    {1, 0x41}
};

MG996R Servo[] = {
    //FrontLeft
    { PWMarray[0].PWM[0],  TowerPro_MG996R_cfg },   //Tiba
    { PWMarray[0].PWM[1],  TowerPro_MG996R_cfg },   //Femur
    { PWMarray[0].PWM[2],  TowerPro_MG996R_cfg, CCW},   //Coxa

    //FrontRight
    { PWMarray[0].PWM[15], TowerPro_MG996R_cfg, CCW },  //Tiba
    { PWMarray[0].PWM[14], TowerPro_MG996R_cfg, CCW },  //Femur
    { PWMarray[0].PWM[13], TowerPro_MG996R_cfg, CCW },  //Coxa

    //MidleLeft
    { PWMarray[0].PWM[4],  TowerPro_MG996R_cfg },       //Tiba
    { PWMarray[0].PWM[5],  TowerPro_MG996R_cfg },       //Femur
    { PWMarray[0].PWM[6],  TowerPro_MG996R_cfg, CCW},   //Coxa

    //MidleRight
    { PWMarray[0].PWM[11], TowerPro_MG996R_cfg, CCW },  //Tiba
    { PWMarray[0].PWM[10], TowerPro_MG996R_cfg, CCW },  //Femur
    { PWMarray[0].PWM[9],  TowerPro_MG996R_cfg, CCW },  //Coxa

    //BackLeft
    { PWMarray[1].PWM[15],  TowerPro_MG996R_cfg },      //Tiba
    { PWMarray[1].PWM[14],  TowerPro_MG996R_cfg },      //Femur
    { PWMarray[1].PWM[13],  TowerPro_MG996R_cfg, CCW }, //Coxa

    //BackRight
    { PWMarray[1].PWM[0], TowerPro_MG996R_cfg, CCW },   //Tiba
    { PWMarray[1].PWM[1], TowerPro_MG996R_cfg, CCW },   //Femur
    { PWMarray[1].PWM[2], TowerPro_MG996R_cfg, CCW },   //Coxa
};

ServoJoint FrontLeft[] = {
    Servo[0],   //Tiba
    Servo[1],   //Femur
    Servo[2]    //Coxa
};

ServoJoint FrontRight[] = {
    Servo[3],   //Tiba
    Servo[4],   //Femur
    Servo[5]    //Coxa
};

ServoJoint MidleLeft[] = {
    Servo[6],   //Tiba
    Servo[7],   //Femur
    Servo[8]    //Coxa
};

ServoJoint MidleRight[] = {
    Servo[9],   //Tiba
    Servo[10],   //Femur
    Servo[11]    //Coxa
};

ServoJoint BackLeft[] = {
    Servo[12],   //Tiba
    Servo[13],   //Femur
    Servo[14]    //Coxa
};

ServoJoint BackRight[] = {
    Servo[15],   //Tiba
    Servo[16],   //Femur
    Servo[17]    //Coxa
};

#define nameLeg(v)  {v[2], v[1], v[0] }

ArachnidLeg leg[] = {
    nameLeg(FrontLeft),
    nameLeg(FrontRight),
    nameLeg(MidleLeft),
    nameLeg(MidleRight),
    nameLeg(BackLeft),
    nameLeg(BackRight),
};


void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us) {
    // PWM[0].setFreq_Hz(freq);
    // printf("new freq %f\n", PWM[0].getFreq_Hz());
    // PWM[0].wakeUp();
    // for(int i = min_dur; i < max_dur; i+=1){
    //     usleep(delay_us);
    //     PWM[0].setDutyCycle(0, 0, i);
    //     // PWM[0].setDutyCycle(1, 0, i);
    // }
    // PWM[0].setDutyCycle(0, 0, (min_dur + max_dur) / 2);
    // printf("mid is %d\n", (min_dur + max_dur) / 2);
}
#define ANGLE_MID   (90.0f)

void testLegs() {
    PWMarray[0].wakeUp();
    PWMarray[1].wakeUp();
    PWMarray[0].setFreq_Hz((Hertz)100);
    PWMarray[1].setFreq_Hz((Hertz)100);
    Servo[0].setAngle(deg(90));
    // joint[0].setPhaseShift(3500);
    // PWM[0].wakeUp();
    // PWM[1].wakeUp();
    // printf("first\n");
    // BR.setJointAngles( 15.0, 15.0, ANGLE_MID); usleep(1000 * 2000);
    // printf("second\n");
    // BR.setJointAngles( 90.0, 90.0, ANGLE_MID); usleep(1000 * 2000);
    // printf("third\n");
    // BR.setJointAngles(170.0, 170.0, ANGLE_MID); usleep(1000 * 2000);

    // // CR.setJointAngles( 55.0, 189.0f, 180.0f); //usleep(1000 * 2000);
    // // BR.setJointAngles( 15.0, 185.0f, 165.0f); usleep(1000 * 5000);
    // CR.deactivate();
    // BR.deactivate();

    //     // CR.setJointAngles(5.0, 5.0, 5.0);
    // // FR.setJointAngles(5.0, 5.0, 5.0);
    // // usleep(1000 * 3000);
    // // BR.setJointAngles(5.0, 5.0, 5.2);
    // // CR.setJointAngles(5.0, 5.0, 5.2);
    // // FR.setJointAngles(5.0, 5.0, 5.2);
    // // usleep(1000 * 3000);
    // // BR.setJointAngles(5.0, 5.0, 5.2);
    // // CR.setJointAngles(5.0, 5.0, 5.2);
    // // FR.setJointAngles(5.0, 5.0, 5.2);
    // // usleep(1000 * 10000);

    // PWM[0].sleepMode();
    // PWM[1].sleepMode();
}
