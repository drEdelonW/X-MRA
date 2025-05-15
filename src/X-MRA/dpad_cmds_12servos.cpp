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
    PWMarray[0].setFreq_Hz((Hertz)50);
    PWMarray[1].setFreq_Hz((Hertz)50);
}
void _off() {
    LOG("OFF\n");
    PWMarray[0].sleepMode();
    PWMarray[1].sleepMode();
}
void _1()  { SV_ALL(  0.0f) }
void _2()  { SV_ALL(-10.0f) }
void _3()  { SV_ALL(-20.0f) }
void _4()  { SV_ALL(-30.0f) }
void _5()  { SV_ALL(-40.0f) }
void _6()  { SV_ALL(-50.0f) }
void _7()  { SV_ALL(-60.0f) }
void _8()  { SV_ALL(-70.0f) }
void _9()  { SV_ALL(-80.0f) }
void _10() { SV_ALL(-90.0f) }

void _1on() { Servo[1].enable(); }
void _1off() { Servo[1].disable(); }

void _2on() { Servo[2].enable(); }
void _2off() { Servo[2].disable(); }

void _left() { SV_ALL(_curAngle - 1) }
void _right() { SV_ALL(_curAngle + 1) }


KeyFunction fArray[KEY_COUNT] = {
    /*KEY_UNKNOWN*/     {},

    /*KEY_LEFT*/        {_left},
    /*KEY_RIGHT*/       {_right},
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

    /*KEY_0*/ {_10},
    /*KEY_1*/ {_1},
    /*KEY_2*/ {_2},
    /*KEY_3*/ {_3},
    /*KEY_4*/ {_4},
    /*KEY_5*/ {_5},
    /*KEY_6*/ {_6},
    /*KEY_7*/ {_7},
    /*KEY_8*/ {_8},
    /*KEY_9*/ {_9},

    /*KEY__0*/ {},
    /*KEY__1*/ {_on},
    /*KEY__2*/ {_off},
    /*KEY__3*/ {_1on},
    /*KEY__4*/ {_1off},
    /*KEY__5*/ {_2on},
    /*KEY__6*/ {_2off},
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
