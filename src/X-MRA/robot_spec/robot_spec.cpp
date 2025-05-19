#include <stdio.h>
#include <unistd.h>
#include "robot_spec.hpp"

#define CFG_1   TowerPro_MG996R_cfg

PCA9685 PWMarray[] = {
    {1, 0x40},
    {1, 0x41}
};

MG996R Servo[] = {
    //FrontLeft
    { PWMarray[0].PWM[0],  CFG_1 },     //Tiba
    { PWMarray[0].PWM[1],  CFG_1 },     //Femur
    { PWMarray[0].PWM[2],  CFG_1, CCW}, //Coxa

    //FrontRight
    { PWMarray[0].PWM[15], CFG_1, CCW },//Tiba
    { PWMarray[0].PWM[14], CFG_1, CCW },//Femur
    { PWMarray[0].PWM[13], CFG_1, CCW },//Coxa

    //MidleLeft
    { PWMarray[0].PWM[4],  CFG_1 },     //Tiba
    { PWMarray[0].PWM[5],  CFG_1 },     //Femur
    { PWMarray[0].PWM[6],  CFG_1, CCW}, //Coxa

    //MidleRight
    { PWMarray[0].PWM[11], CFG_1, CCW },//Tiba
    { PWMarray[0].PWM[10], CFG_1, CCW },//Femur
    { PWMarray[0].PWM[9],  CFG_1, CCW },//Coxa

    //BackLeft
    { PWMarray[1].PWM[15], CFG_1 },     //Tiba
    { PWMarray[1].PWM[14], CFG_1 },     //Femur
    { PWMarray[1].PWM[13], CFG_1, CCW },//Coxa

    //BackRight
    { PWMarray[1].PWM[0],  CFG_1, CCW },//Tiba
    { PWMarray[1].PWM[1],  CFG_1, CCW },//Femur
    { PWMarray[1].PWM[2],  CFG_1, CCW },//Coxa
};

ServoJoint FrontLeft[] = {
    {Servo[0],  deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[1],  deg(-105), deg(105), deg(0)}, //Femur
    {Servo[2],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint FrontRight[] = {
    {Servo[3],  deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[4],  deg(-105), deg(105), deg(0)}, //Femur
    {Servo[5],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint MidleLeft[] = {
    {Servo[6],  deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[7],  deg(-105), deg(105), deg(0)}, //Femur
    {Servo[8],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint MidleRight[] = {
    {Servo[9],  deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[10], deg(-105), deg(105), deg(0)}, //Femur
    {Servo[11], deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint BackLeft[] = {
    {Servo[12], deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[13], deg(-105), deg(105), deg(0)}, //Femur
    {Servo[14], deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint BackRight[] = {
    {Servo[15], deg(-105), deg(105), deg(0)}, //Tiba
    {Servo[16], deg(-105), deg(105), deg(0)}, //Femur
    {Servo[17], deg(-105), deg(105), deg(0)}, //Coxa
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
