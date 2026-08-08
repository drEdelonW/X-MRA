#include "dpad.h"
#include "common_tools.h"

#include "X-MRA.hpp"

#if 0
#   define servoNum DIMOF(Servo)
#else
#   define servoNum (6*3)
#endif

#include "PS_MG996R.hpp"    // MG996R
extern MG996R Servo[];

static float _curAngle = 0.f;
#if 1
void SV_ALL(float ang) {
    LOG("[%.2f]\n", (_curAngle = ang));
    for(int idxSv = 0; idxSv < servoNum; idxSv++)
        Servo[idxSv].setAngle(deg(ang));
}
#else
#define SV_ALL(v)                                   \
    LOG("[%.2f]\n", (v));                           \
    _curAngle = (v);                                \
    for(int idxSv = 0; idxSv < servoNum; idxSv++)   \
        Servo[idxSv].setAngle(deg(v));
#endif

void _on()  {
    LOG("ON\n");
    PowerAllow(true);
}
void _off() {
    LOG("OFF\n");
    PowerAllow(false);
}
void _1()  { SV_ALL(  0.0f); }
void _2()  { SV_ALL(-10.0f); }
void _3()  { SV_ALL(-20.0f); }
void _4()  { SV_ALL(-30.0f); }
void _5()  { SV_ALL(-40.0f); }
void _6()  { SV_ALL(-50.0f); }
void _7()  { SV_ALL(-60.0f); }
void _8()  { SV_ALL(-70.0f); }
void _9()  { SV_ALL(-80.0f); }
void _10() { SV_ALL(-90.0f); }

void _1on()  { Servo[1].enable(); }
void _1off() { Servo[1].disable(); }

void _2on()  { Servo[2].enable(); }
void _2off() { Servo[2].disable(); }

void _left()  { SV_ALL(_curAngle - 1); }
void _right() { SV_ALL(_curAngle + 1); }


KeyFunction fArray[KEY_COUNT] = {
    [KEY_UNKNOWN] = 0,

    [KEY_LEFT]          = _left,
    [KEY_RIGHT]         = _right,
    [KEY_UP] = 0,
    [KEY_DOWN] = 0,

    [KEY__LEFT] = 0,
    [KEY__RIGHT] = 0,
    [KEY__UP] = 0,
    [KEY__DOWN] = 0,

    [KEY_INSERT] = 0,
    [KEY_DELETE] = 0,
    [KEY_PAGE_UP] = 0,
    [KEY_PAGE_DOWN] = 0,
    [KEY_HOME] = 0,
    [KEY_END] = 0,

    [KEY_BACKSPACE] = 0,
    [KEY_ENTER] = 0,
    [KEY_SPACE] = 0,
    [KEY_TAB] = 0,
    [KEY_ESCAPE] = 0,     // used for exit
    [KEY_TILDA] = 0,

    [KEY_F1] = 0,
    [KEY_F2] = 0,
    [KEY_F3] = 0,
    [KEY_F4] = 0,
    [KEY_F5] = 0,
    [KEY_F6] = 0,
    [KEY_F7] = 0,
    [KEY_F8] = 0,
    [KEY_F9] = 0,
    [KEY_F10] = 0,
    [KEY_F11] = 0,
    [KEY_F12] = 0,

    [KEY_0]     = _10,
    [KEY_1]     = _1,
    [KEY_2]     = _2,
    [KEY_3]     = _3,
    [KEY_4]     = _4,
    [KEY_5]     = _5,
    [KEY_6]     = _6,
    [KEY_7]     = _7,
    [KEY_8]     = _8,
    [KEY_9]     = _9,

    [KEY__0] = 0,
    [KEY__1]    = _on,
    [KEY__2]    = _off,
    [KEY__3]    = _1on,
    [KEY__4]    = _1off,
    [KEY__5]    = _2on,
    [KEY__6]    = _2off,
    [KEY__7] = 0,
    [KEY__8] = 0,
    [KEY__9] = 0,

    [KEY_A] = 0,
    [KEY_B] = 0,
    [KEY_C] = 0,
    [KEY_D] = 0,
    [KEY_E] = 0,
    [KEY_F] = 0,
    [KEY_G] = 0,
    [KEY_H] = 0,
    [KEY_I] = 0,
    [KEY_J] = 0,
    [KEY_K] = 0,
    [KEY_L] = 0,
    [KEY_M] = 0,
    [KEY_N] = 0,
    [KEY_O] = 0,
    [KEY_P] = 0,
    [KEY_Q] = 0,
    [KEY_R] = 0,
    [KEY_S] = 0,
    [KEY_T] = 0,
    [KEY_U] = 0,
    [KEY_V] = 0,
    [KEY_W] = 0,
    [KEY_X] = 0,
    [KEY_Y] = 0,
    [KEY_Z] = 0,
    //KEY_COUNT
};
