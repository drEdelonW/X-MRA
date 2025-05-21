#include "dpad.h"
#include "common_tools.h"
#include <unistd.h>  //usleep

#include "robot_spec.hpp"
#include "Vector3d.hpp"

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
    if (XMRA.setOffs(v)) \
        XMRA.applyPose(); \
// #define LEG_PON_ALL()  {for(int i = 0; i < NUM_LEGS; i++) { XMRA._legs[i].engage(); }}
// #define LEG_POFF_ALL() {for(int i = 0; i < NUM_LEGS; i++) { XMRA._legs[i].release(); }}

void _on()  {
    if (XMRA.isArmed()) return;
    LOG("ON\n");
    PWMarray[0].wakeUp();
    PWMarray[1].wakeUp();
    PWMarray[0].setFreq_Hz((Hertz)300);
    PWMarray[1].setFreq_Hz((Hertz)300);
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

const Millimeters coxaLength  = (Millimeters) 27.0f;
const Millimeters femurLength = (Millimeters) 85.1f;
const Millimeters tibiaLength = (Millimeters)144.23f;

const Millimeters tibiaX      = (Millimeters) 27.33f;
const Millimeters tibiaZ      = (Millimeters)141.6f;

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
    /*KEY_UNKNOWN*/     {},

    /*KEY_LEFT*/        {_yPos},
    /*KEY_RIGHT*/       {_yNeg},
    /*KEY_UP*/          {_xPos},
    /*KEY_DOWN*/        {_xNeg},

    /*KEY__LEFT*/       {},
    /*KEY__RIGHT*/      {},
    /*KEY__UP*/         {_zPos},
    /*KEY__DOWN*/       {_zNeg},

    /*KEY_INSERT*/      {},
    /*KEY_DELETE*/      {},
    /*KEY_PAGE_UP*/     {_on},
    /*KEY_PAGE_DOWN*/   {_off},
    /*KEY_HOME*/        {_goGome},
    /*KEY_END*/         {},

    /*KEY__INSERT*/      {},
    /*KEY__DELETE*/      {},
    /*KEY__PAGE_UP*/     {},
    /*KEY__PAGE_DOWN*/   {},
    /*KEY__HOME*/        {},
    /*KEY__END*/         {},

    /*KEY_BACKSPACE*/   {},
    /*KEY_ENTER*/       {},
    /*KEY_TAB*/         {},
    /*KEY_SPACE*/       {},
    /*KEY_ESCAPE*/      {},     // used for exit
    /*KEY_TILDA*/       {},

    /*KEY_F1*/  {},
    /*KEY_F2*/  {},
    /*KEY_F3*/  {},
    /*KEY_F4*/  {},
    /*KEY_F5*/  {},
    /*KEY_F6*/  {},
    /*KEY_F7*/  {},
    /*KEY_F8*/  {},
    /*KEY_F9*/  {},
    /*KEY_F10*/ {},
    /*KEY_F11*/ {},
    /*KEY_F12*/ {},

    /*KEY_0*/ {},
    /*KEY_1*/ {_1},
    /*KEY_2*/ {_2},
    /*KEY_3*/ {_3},
    /*KEY_4*/ {},
    /*KEY_5*/ {},
    /*KEY_6*/ {},
    /*KEY_7*/ {},
    /*KEY_8*/ {},
    /*KEY_9*/ {},

    /*KEY__0*/ {},
    /*KEY__1*/ {},
    /*KEY__2*/ {},
    /*KEY__3*/ {},
    /*KEY__4*/ {},
    /*KEY__5*/ {},
    /*KEY__6*/ {},
    /*KEY__7*/ {},
    /*KEY__8*/ {},
    /*KEY__9*/ {},

    /*KEY_A*/ {},
    /*KEY_B*/ {},
    /*KEY_C*/ {},
    /*KEY_D*/ {},
    /*KEY_E*/ {},
    /*KEY_F*/ {},
    /*KEY_G*/ {},
    /*KEY_H*/ {},
    /*KEY_I*/ {_info},
    /*KEY_J*/ {},
    /*KEY_K*/ {},
    /*KEY_L*/ {},
    /*KEY_M*/ {},
    /*KEY_N*/ {},
    /*KEY_O*/ {},
    /*KEY_P*/ {},
    /*KEY_Q*/ {},
    /*KEY_R*/ {},
    /*KEY_S*/ {},
    /*KEY_T*/ {},
    /*KEY_U*/ {},
    /*KEY_V*/ {},
    /*KEY_W*/ {},
    /*KEY_X*/ {},
    /*KEY_Y*/ {},
    /*KEY_Z*/ {},

    /*KEY__A*/ {},
    /*KEY__B*/ {},
    /*KEY__C*/ {},
    /*KEY__D*/ {},
    /*KEY__E*/ {},
    /*KEY__F*/ {},
    /*KEY__G*/ {},
    /*KEY__H*/ {},
    /*KEY__I*/ {},
    /*KEY__J*/ {},
    /*KEY__K*/ {},
    /*KEY__L*/ {},
    /*KEY__M*/ {},
    /*KEY__N*/ {},
    /*KEY__O*/ {},
    /*KEY__P*/ {},
    /*KEY__Q*/ {},
    /*KEY__R*/ {},
    /*KEY__S*/ {},
    /*KEY__T*/ {},
    /*KEY__U*/ {},
    /*KEY__V*/ {},
    /*KEY__W*/ {},
    /*KEY__X*/ {},
    /*KEY__Y*/ {},
    /*KEY__Z*/ {},

    //KEY_COUNT
};
