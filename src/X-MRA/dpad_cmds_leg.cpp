#include "dpad.h"
#include "common_tools.h"

#include "robot_spec.hpp"

static float _curAngle = 0;

#define SV_ALL(v)  \
    LOG("[%.2f]\n", (v)); \
    _curAngle = (v); \
    for(int i = 0; i < DIMOF(Servo); i++) {\
        Servo[i].setAngle(deg(v));\
    }

void _on()  {
    LOG("ON\n");
    PWMarray[0].wakeUp();
    PWMarray[1].wakeUp();
    PWMarray[0].setFreq_Hz((Hertz)300);
    PWMarray[1].setFreq_Hz((Hertz)300);
}
void _off() {
    LOG("OFF\n");
    PWMarray[0].sleepMode();
    PWMarray[1].sleepMode();
}

void _u()   { leg[0].setJointAngles(deg(0), deg(0), deg(0));    leg[1].setJointAngles(deg(0), deg(0), deg(0)); }
void _d()   { leg[0].setJointAngles(deg(10), deg(10), deg(10)); leg[1].setJointAngles(deg(10), deg(10), deg(10)); }

void __u()  { leg[0].setTipPosition( 0.0,  0.0,  0.0); leg[1].setTipPosition( 0.0,  0.0,  0.0); }

void __d()  { leg[0].setTipPosition( 150.0,  100.0,  0.0); leg[1].setTipPosition( 150.0,  100.0,  0.0); }
void __d1() { leg[0].setTipPosition( 150.0,   50.0,  0.0); leg[1].setTipPosition( 150.0,   50.0,  0.0); }
void __d2() { leg[0].setTipPosition( 150.0,    0.0,  0.0); leg[1].setTipPosition( 150.0,    0.0,  0.0); }
void __d3() { leg[0].setTipPosition( 150.0,  -50.0,  0.0); leg[1].setTipPosition( 150.0,  -50.0,  0.0); }
void __d4() { leg[0].setTipPosition( 150.0, -100.0,  0.0); leg[1].setTipPosition( 150.0, -100.0,  0.0); }


KeyFunction fArray[KEY_COUNT] = {
    /*KEY_UNKNOWN*/     {},

    /*KEY_LEFT*/        {},
    /*KEY_RIGHT*/       {},
    /*KEY_UP*/          {},
    /*KEY_DOWN*/        {},

    /*KEY_INSERT*/      {},
    /*KEY_DELETE*/      {},
    /*KEY_PAGE_UP*/     {},
    /*KEY_PAGE_DOWN*/   {},
    /*KEY_HOME*/        {},
    /*KEY_END*/         {},
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
    /*KEY_1*/ {_u},
    /*KEY_2*/ {_d},
    /*KEY_3*/ {__u},
    /*KEY_4*/ {__d},
    /*KEY_5*/ {__d1},
    /*KEY_6*/ {__d2},
    /*KEY_7*/ {__d3},
    /*KEY_8*/ {__d4},
    /*KEY_9*/ {},

    /*KEY__0*/ {},
    /*KEY__1*/ {_on},
    /*KEY__2*/ {_off},
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
    /*KEY_I*/ {},
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
    //KEY_COUNT
};
