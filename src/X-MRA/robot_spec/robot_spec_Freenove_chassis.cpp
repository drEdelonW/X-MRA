#include "X-MRA.hpp"

#include "PA_PCA9685.hpp"
PCA9685 PWMarray[] = {
    {1, PCAaddr_0},
    {1, PCAaddr_1}
};

#include "PS_MG996R.hpp"
// #define CFG_1 TowerPro_MG996R_cfg
#define CFG_1 FreeNove_noname_cfg
MG996R Servo[] = {
    //FrontLeft
    { PWMarray[0].PWM[PwmChD],  CFG_1, CW }, //Tibia
    { PWMarray[0].PWM[PwmChE],  CFG_1, CW }, //Femur
    { PWMarray[0].PWM[PwmChF],  CFG_1, CCW}, //Coxa

    //FrontRight
    { PWMarray[1].PWM[PwmCh0],  CFG_1, CCW}, //Tibia
    { PWMarray[1].PWM[PwmCh1],  CFG_1, CCW}, //Femur
    { PWMarray[1].PWM[PwmCh2],  CFG_1, CCW}, //Coxa

    //MidleLeft
    { PWMarray[0].PWM[PwmCh5],  CFG_1, CW }, //Tibia
    { PWMarray[0].PWM[PwmCh6],  CFG_1, CW }, //Femur
    { PWMarray[0].PWM[PwmCh7],  CFG_1, CCW}, //Coxa

    //MidleRight
    { PWMarray[1].PWM[PwmCh8],  CFG_1, CCW}, //Tibia
    { PWMarray[1].PWM[PwmCh9],  CFG_1, CCW}, //Femur
    { PWMarray[1].PWM[PwmChA],  CFG_1, CCW}, //Coxa

    //BackLeft
    { PWMarray[0].PWM[PwmCh0],  CFG_1, CW }, //Tibia
    { PWMarray[0].PWM[PwmCh1],  CFG_1, CW }, //Femur
    { PWMarray[0].PWM[PwmCh2],  CFG_1, CCW}, //Coxa

    //BackRight
    { PWMarray[1].PWM[PwmChD],  CFG_1, CCW}, //Tibia
    { PWMarray[1].PWM[PwmChE],  CFG_1, CCW}, //Femur
    { PWMarray[1].PWM[PwmChF],  CFG_1, CCW}, //Coxa
};


#include "pwmServoJoint.hpp"
ServoJoint FrontLeft[] = {
    {Servo[0],  deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[1],  deg(-105), deg(105), deg(35)},//Femur
    {Servo[2],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint FrontRight[] = {
    {Servo[3],  deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[4],  deg(-105), deg(105), deg(35)},//Femur
    {Servo[5],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint MidleLeft[] = {
    {Servo[6],  deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[7],  deg(-105), deg(105), deg(35)},//Femur
    {Servo[8],  deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint MidleRight[] = {
    {Servo[9],  deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[10], deg(-105), deg(105), deg(35)},//Femur
    {Servo[11], deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint BackLeft[] = {
    {Servo[12], deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[13], deg(-105), deg(105), deg(35)},//Femur
    {Servo[14], deg(-105), deg(105), deg(0)}, //Coxa
};

ServoJoint BackRight[] = {
    {Servo[15], deg(-105), deg(105), deg(0)}, //Tibia
    {Servo[16], deg(-105), deg(105), deg(35)},//Femur
    {Servo[17], deg(-105), deg(105), deg(0)}, //Coxa
};

#define nameLeg(v)  v[2], v[1], v[0]
ArachnidLeg leg[] = {
    {nameLeg(FrontLeft),  mm( 84.36f), deg(-27.63f)},
    {nameLeg(FrontRight), mm( 84.36f), deg( 27.63f)},
    {nameLeg(MidleLeft),  mm( 39.25f), deg(-90.0f)},
    {nameLeg(MidleRight), mm( 39.25f), deg( 90.0f)},
    {nameLeg(BackLeft),   mm( 84.36f), deg( 27.63f + 180.0f)},
    {nameLeg(BackRight),  mm( 84.36f), deg(-27.63f + 180.0f)},
};

ArachnidBody XMRA(leg, sizeof(leg)/sizeof(leg[0]));