#include "dpad.h"
#include "common_tools.h"

#include "robot_spec.hpp"



void _on()  { LOG("ON\n");  PWMarray.wakeUp(); PWMarray.setFreq_Hz((Hertz)100);}
void _off() { LOG("OFF\n"); PWMarray.sleepMode();}
void _1() {
    LOG("1\n");
    Servo.setAngle(deg(0));
    Servo1.setAngle(deg(0));
    Servo2.setAngle(deg(0));
}
void _2() {
    LOG("2\n");
    Servo.setAngle(deg(90));
    Servo1.setAngle(deg(90));
    Servo2.setAngle(deg(90));
}
void _3() {
    LOG("3\n");
    Servo.setAngle(deg(180));
    Servo1.setAngle(deg(180));
    Servo2.setAngle(deg(180));
}


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
    /*KEY_1*/ {_on},
    /*KEY_2*/ {_off},
    /*KEY_3*/ {_1},
    /*KEY_4*/ {_2},
    /*KEY_5*/ {_3},
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
