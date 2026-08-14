#include "X-MRA.hpp"

// https://store.freenove.com/products/fnk0052

#include "GPIO.hpp"
GpioChip chip;

#define BUZZER (17)
GpioLine Buzzer(chip,  BUZZER);
void beep(bool bz) { Buzzer.setB(bz); }

#include "i2cBus_EndPiont.hpp"
i2cBus iBus(1, true);

#include "PA_PCA9685.hpp"
PCA9685 PWMarray[] = {
    {iBus, PCAaddr_1},
    {iBus, PCAaddr_0}
};
#define PwCh(n)  PWMarray[(n & 0xF0) >> 4].PWM[n & 0x0F]

#define PWR_LOAD (4)
GpioLine PwrLoad(chip, PWR_LOAD);
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

#include "PS_FreeNove.hpp"  // sFreeNove
sFreeNove Servo[] = {
    //FrontLeft
    [0]  = {PwCh(9),  CCW}, //Coxa
    [1]  = {PwCh(8),  CW }, //Femur
    [2]  = {PwCh(31), CW }, //Tibia

    //FrontRight
    [3]  = {PwCh(22), CCW}, //Coxa
    [4]  = {PwCh(23), CCW}, //Femur
    [5]  = {PwCh(27), CCW}, //Tibia

    //MidleLeft
    [6]  = {PwCh(12), CCW}, //Coxa
    [7]  = {PwCh(11), CW }, //Femur
    [8]  = {PwCh(10), CW }, //Tibia

    //MidleRight
    [9]  = {PwCh(19), CCW}, //Coxa
    [10] = {PwCh(20), CCW}, //Femur
    [11] = {PwCh(21), CCW}, //Tibia

    //BackLeft
    [12] = {PwCh(13), CCW}, //Coxa
    [13] = {PwCh(14), CW }, //Femur
    [14] = {PwCh(15), CW }, //Tibia

    //BackRight
    [15] = {PwCh(16), CCW}, //Coxa
    [16] = {PwCh(17), CCW}, //Femur
    [17] = {PwCh(18), CCW}, //Tibia
};

#include "pwmServoJoint.hpp"

sLeg_t FrontLeft = {
    [Coxa]  = { .jn = (ServoJoint{Servo[0], deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[1], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[2], deg(0)}),  .len = mm(144.23f)}
};

sLeg_t FrontRight = {
    [Coxa]  = { .jn = (ServoJoint{Servo[3], deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[4], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[5], deg(0)}),  .len = mm(144.23f)}
};

sLeg_t MidleLeft = {
    [Coxa]  = { .jn = (ServoJoint{Servo[6], deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[7], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[8], deg(0)}),  .len = mm(144.23f)}
};

sLeg_t MidleRight = {
    [Coxa]  = { .jn = (ServoJoint{Servo[9],  deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[10], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[11], deg(0)}),  .len = mm(144.23f)}
};

sLeg_t BackLeft = {
    [Coxa]  = { .jn = (ServoJoint{Servo[12], deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[13], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[14], deg(0)}),  .len = mm(144.23f)}
};

sLeg_t BackRight = {
    [Coxa]  = { .jn = (ServoJoint{Servo[15], deg(0)}),  .len = mm( 27.f  )},
    [Femur] = { .jn = (ServoJoint{Servo[16], deg(35)}), .len = mm( 85.1f )},
    [Tibia] = { .jn = (ServoJoint{Servo[17], deg(0)}),  .len = mm(144.23f)}
};

ArachnidLeg leg[] = {
    {FrontLeft,  mm( 84.36f), deg(-27.63f)},
    {FrontRight, mm( 84.36f), deg( 27.63f)},
    {MidleLeft,  mm( 39.25f), deg(-90.f)},
    {MidleRight, mm( 39.25f), deg( 90.f)},
    {BackLeft,   mm( 84.36f), deg( 27.63f + 180.f)},
    {BackRight,  mm( 84.36f), deg(-27.63f + 180.f)},
};

ArachnidBody XMRA(leg, sizeof(leg)/sizeof(leg[0]));