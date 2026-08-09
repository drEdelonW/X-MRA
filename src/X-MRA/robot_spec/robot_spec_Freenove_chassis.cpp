#include "X-MRA.hpp"

// https://store.freenove.com/products/fnk0052

#include "GPIO.hpp"
#define PWR_LOAD    (4)
#define BUZZER      (17)

GpioChip chip;

GpioLine PwrLoad(chip, PWR_LOAD);
GpioLine Buzzer(chip,  BUZZER);

void beep(bool bz) { Buzzer.setB(bz); }

#include "i2cBus_EndPiont.hpp"
i2cBus iBus(1, true);

#include "PA_PCA9685.hpp"
PCA9685 PWMarray[] = {
    {iBus, PCAaddr_1},
    {iBus, PCAaddr_0}
};

void PowerAllow(bool alw) {
    if (!alw) {
        PWMarray[0].sleepMode();
        PWMarray[1].sleepMode();
    }
    PwrLoad.setB(!alw);
    if (alw) {
        PWMarray[0].wakeUp();
        PWMarray[1].wakeUp();
        PWMarray[0].setFreq_Hz(Hz(300));
        PWMarray[1].setFreq_Hz(Hz(300));
    }
}

#define PwCh(n)  PWMarray[(n & 0xF0) >> 4].PWM[n & 0x0F]

#define FreeNove_noname_cfg     us(560), us(2650), deg(160), deg(90)
#define CFG_1 FreeNove_noname_cfg

#include "PS_MG996R.hpp"
MG996R Servo[] = {
    //FrontLeft
    { PwCh(9),  CFG_1, CW }, //Tibia
    { PwCh(8),  CFG_1, CW }, //Femur
    { PwCh(31), CFG_1, CCW}, //Coxa

    //FrontRight
    { PwCh(22),  CFG_1, CCW}, //Tibia
    { PwCh(23),  CFG_1, CCW}, //Femur
    { PwCh(27),  CFG_1, CCW}, //Coxa

    //MidleLeft
    { PwCh(12),  CFG_1, CW }, //Tibia
    { PwCh(11),  CFG_1, CW }, //Femur
    { PwCh(10),  CFG_1, CCW}, //Coxa

    //MidleRight
    { PwCh(19),  CFG_1, CCW}, //Tibia
    { PwCh(20),  CFG_1, CCW}, //Femur
    { PwCh(21),  CFG_1, CCW}, //Coxa

    //BackLeft
    { PwCh(15),  CFG_1, CW }, //Tibia
    { PwCh(14),  CFG_1, CW }, //Femur
    { PwCh(13),  CFG_1, CCW}, //Coxa

    //BackRight
    { PwCh(16),  CFG_1, CCW}, //Tibia
    { PwCh(17),  CFG_1, CCW}, //Femur
    { PwCh(18),  CFG_1, CCW}, //Coxa
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
    {nameLeg(MidleLeft),  mm( 39.25f), deg(-90.f)},
    {nameLeg(MidleRight), mm( 39.25f), deg( 90.f)},
    {nameLeg(BackLeft),   mm( 84.36f), deg( 27.63f + 180.f)},
    {nameLeg(BackRight),  mm( 84.36f), deg(-27.63f + 180.f)},
};

ArachnidBody XMRA(leg, sizeof(leg)/sizeof(leg[0]));