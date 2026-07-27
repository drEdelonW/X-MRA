#include "dpad.h"
#include "common_tools.h"
#include <unistd.h>  //usleep

#include "X-MRA.hpp"
#include "Vector3d.hpp"

#include "PA_PCA9685.hpp"
extern PCA9685 PWMarray[];


static float _curAngle = 0;

#define NUM_LEGS    (6)

// #define SV_ALL(v)  \
//     LOG("[%.2f]\n", (v)); \
//     _curAngle = (v); \
//     for(int i = 0; i < DIMOF(Servo); i++) {\
//         Servo[i].setAngle(deg(v));\
//     }

#define LEG_J_ALL(v1, v2, v3)  \
    for(int i = 0; i < NUM_LEGS; i++) { \
        if (XMRA._legs[i].checkJointAngles(deg(v1),deg(v2),deg(v3))) \
            XMRA._legs[i].applyPose(); \
    }

// #define LEG_P_ALL(v1, v2, v3)  \
//     for(int i = 0; i < NUM_LEGS; i++) { \
//         if (XMRA._legs[i].checkTipPosition(v1,v2,v3)) \
//             XMRA._legs[i].applyPose(); \
//     }

#define LEG_PV_ALL(v)  \
    for(int i = 0; i < NUM_LEGS; i++) { \
        if (XMRA._legs[i].checkTipPosition(v)) \
            XMRA._legs[i].applyPose(); \
    }
#define MRA_PV_ALL(v)  \
    XMRA.trySetOffs(v);

// #define LEG_PON_ALL()  {for(int i = 0; i < NUM_LEGS; i++) { XMRA._legs[i].engage(); }}
// #define LEG_POFF_ALL() {for(int i = 0; i < NUM_LEGS; i++) { XMRA._legs[i].release(); }}

void _on()  {
    if (XMRA.isArmed()) return;
    LOG("ON\n");
    PWMarray[0].wakeUp();
    PWMarray[1].wakeUp();
    PWMarray[0].setFreq_Hz(Hz(300));
    PWMarray[1].setFreq_Hz(Hz(300));
    XMRA.ARM();
}
void _off() {
    LOG("OFF\n");
    XMRA.DISARM();
    usleep(1000);

    PWMarray[0].sleepMode();
    PWMarray[1].sleepMode();
}

Millimeters mStep = 1.0f;

void _1()  { mStep =  0.1f; }
void _2()  { mStep =  1.0f; }
void _3()  { mStep = 10.0f; }

const Millimeters coxaLength  = mm( 27.0f);
const Millimeters femurLength = mm( 85.1f);
const Millimeters tibiaLength = mm(144.23f);

const Millimeters tibiaX      = mm( 27.33f);
const Millimeters tibiaZ      = mm(141.6f);

// #define DSTx    (coxaLength + femurLength + tibiaX)
// #define DSTy    (0.0f)
// #define DSTz    (-tibiaZ)

#define DSTx    (0.0f)
#define DSTy    (0.0f)
#define DSTz    (0.0f)

Vector3D _curPose = {DSTx, DSTy, DSTz};

void _xPos() { _curPose.x += mStep; MRA_PV_ALL(_curPose); }
void _xNeg() { _curPose.x -= mStep; MRA_PV_ALL(_curPose); }

void _yPos() { _curPose.y -= mStep; MRA_PV_ALL(_curPose); }
void _yNeg() { _curPose.y += mStep; MRA_PV_ALL(_curPose); }

void _zPos() { _curPose.z += mStep; MRA_PV_ALL(_curPose); }
void _zNeg() { _curPose.z -= mStep; MRA_PV_ALL(_curPose); }

void _goGome()  { _curPose = {DSTx, DSTy, DSTz}; MRA_PV_ALL(_curPose); }

void _info() {
    for (int i = 0; i < 6; i++){
        LOG("[%d] legSpace:", i);
        XMRA._legs[i].tipPosLegSpace(deg(0), deg(0), deg(0)).print();
        LOG("\t bodySpace:");
        XMRA._legs[i].tipPosBodySpace(deg(0), deg(0), deg(0)).print();
        LOG("\n");
    }
}

KeyFunction fArray[KEY_COUNT] = {
    [KEY_UNKNOWN]   = 0,

    [KEY_LEFT]      = _yPos,
    [KEY_RIGHT]     = _yNeg,
    [KEY_UP]        = _xPos,
    [KEY_DOWN]      = _xNeg,

    [KEY__LEFT]     = 0,
    [KEY__RIGHT]    = 0,
    [KEY__UP]       = _zPos,
    [KEY__DOWN]     = _zNeg,

    [KEY_INSERT]    = 0,
    [KEY_DELETE]    = 0,
    [KEY_PAGE_UP]   = _on,
    [KEY_PAGE_DOWN] = _off,
    [KEY_HOME]      = _goGome,
    [KEY_END]       = 0,

    [KEY_BACKSPACE] = 0,
    [KEY_ENTER]     = 0,
    [KEY_SPACE]     = 0,
    [KEY_TAB]       = 0,
    [KEY_ESCAPE]    = 0, // used for exit
    [KEY_TILDA]     = 0,

    [KEY_F1]    = 0,
    [KEY_F2]    = 0,
    [KEY_F3]    = 0,
    [KEY_F4]    = 0,
    [KEY_F5]    = 0,
    [KEY_F6]    = 0,
    [KEY_F7]    = 0,
    [KEY_F8]    = 0,
    [KEY_F9]    = 0,
    [KEY_F10]   = 0,
    [KEY_F11]   = 0,
    [KEY_F12]   = 0,

    [KEY_0]     = 0,
    [KEY_1]     = _1,
    [KEY_2]     = _2,
    [KEY_3]     = _3,
    [KEY_4]     = 0,
    [KEY_5]     = 0,
    [KEY_6]     = 0,
    [KEY_7]     = 0,
    [KEY_8]     = 0,
    [KEY_9]     = 0,

    [KEY__0]    = 0,
    [KEY__1]    = 0,
    [KEY__2]    = 0,
    [KEY__3]    = 0,
    [KEY__4]    = 0,
    [KEY__5]    = 0,
    [KEY__6]    = 0,
    [KEY__7]    = 0,
    [KEY__8]    = 0,
    [KEY__9]    = 0,

    [KEY_A]     = 0,
    [KEY_B]     = 0,
    [KEY_C]     = 0,
    [KEY_D]     = 0,
    [KEY_E]     = 0,
    [KEY_F]     = 0,
    [KEY_G]     = 0,
    [KEY_H]     = 0,
    [KEY_I]     = _info,
    [KEY_J]     = 0,
    [KEY_K]     = 0,
    [KEY_L]     = 0,
    [KEY_M]     = 0,
    [KEY_N]     = 0,
    [KEY_O]     = 0,
    [KEY_P]     = 0,
    [KEY_Q]     = 0,
    [KEY_R]     = 0,
    [KEY_S]     = 0,
    [KEY_T]     = 0,
    [KEY_U]     = 0,
    [KEY_V]     = 0,
    [KEY_W]     = 0,
    [KEY_X]     = 0,
    [KEY_Y]     = 0,
    [KEY_Z]     = 0,

    [KEY__INSERT]       = 0,
    [KEY__DELETE]       = 0,
    [KEY__PAGE_UP]      = 0,
    [KEY__PAGE_DOWN]    = 0,
    [KEY__HOME]         = 0,
    [KEY__END]          = 0,

    [KEY__A] = 0,
    [KEY__B] = 0,
    [KEY__C] = 0,
    [KEY__D] = 0,
    [KEY__E] = 0,
    [KEY__F] = 0,
    [KEY__G] = 0,
    [KEY__H] = 0,
    [KEY__I] = 0,
    [KEY__J] = 0,
    [KEY__K] = 0,
    [KEY__L] = 0,
    [KEY__M] = 0,
    [KEY__N] = 0,
    [KEY__O] = 0,
    [KEY__P] = 0,
    [KEY__Q] = 0,
    [KEY__R] = 0,
    [KEY__S] = 0,
    [KEY__T] = 0,
    [KEY__U] = 0,
    [KEY__V] = 0,
    [KEY__W] = 0,
    [KEY__X] = 0,
    [KEY__Y] = 0,
    [KEY__Z] = 0,

    //KEY_COUNT
};
