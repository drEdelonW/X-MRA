#include "dpad.h"

static void _begin();
dPadBind bS12 = { [KEY_UNKNOWN] = _begin };

#include "common_tools.h"

#include "X-MRA.hpp"
#if 0
#   define servoNum DIMOF(Servo)
#else
#   define servoNum (6*3)
// #   define servoNum (2*3)
#endif

#if 0
#include "PS_MG996R.hpp"    // MG996R
extern MG996R Servo[];
#else
#include "PS_FreeNove.hpp"  // sFreeNove
extern sFreeNove Servo[];
#endif

static float _curAngle = 0.f;
void allSv(float ang) {
    LOG("\r[%.2f]    ", (_curAngle = ang));
    for(int idxSv = 0; idxSv < servoNum; idxSv++)
        Servo[idxSv].setAngle(deg(ang));
}
static void _left()  { allSv(_curAngle - 1); }
static void _right() { allSv(_curAngle + 1); }

static void _1()  { allSv(  0.f); } static void __1()  { allSv(  0.f); }
static void _2()  { allSv(-10.f); } static void __2()  { allSv( 10.f); }
static void _3()  { allSv(-20.f); } static void __3()  { allSv( 20.f); }
static void _4()  { allSv(-30.f); } static void __4()  { allSv( 30.f); }
static void _5()  { allSv(-40.f); } static void __5()  { allSv( 40.f); }
static void _6()  { allSv(-50.f); } static void __6()  { allSv( 50.f); }
static void _7()  { allSv(-60.f); } static void __7()  { allSv( 60.f); }
static void _8()  { allSv(-70.f); } static void __8()  { allSv( 70.f); }
static void _9()  { allSv(-80.f); } static void __9()  { allSv( 80.f); }
static void _10() { allSv(-90.f); } static void __10() { allSv( 90.f); }

static void _on()  { LOG("ON\n");  PowerAllow(true);  }
static void _off() { LOG("OFF\n"); PowerAllow(false); }

static void _1on()  { LOG("1 ON\n");  Servo[1].enable();  }
static void _1off() { LOG("2 OFF\n"); Servo[1].disable(); }

static void _2on()  { LOG("2 ON\n");  Servo[2].enable();  }
static void _2off() { LOG("2 OFF\n"); Servo[2].disable(); }

extern dPadBind bL;
static void _callLegs() { dpad(bL); _begin(); }
#include "ADS7830_cWrap.h"   // adcReadAll()
static void _begin() {
    LOG(CLEAR_SCREEN
        "12 Servo begin\n"
        "q/Q        - On/Off(all)\n"
        "w/W        - On/Off(1)\n"
        "e/D        - On/Off(2)\n"
        "0..9 +/-Sh - setAng(0..9)\n"
        "</>        - +/- Angqel\n"
        "A          - readADC\n"
    );
    bS12[KEY_LEFT]  = _left;
    bS12[KEY_RIGHT] = _right;

    bS12[KEY_ESCAPE] = dPadQuit;
    bS12[KEY_L] = _callLegs;

    bS12[KEY_1] = _1;    bS12[KEY__1] = __1;
    bS12[KEY_2] = _2;    bS12[KEY__2] = __2;
    bS12[KEY_3] = _3;    bS12[KEY__3] = __3;
    bS12[KEY_4] = _4;    bS12[KEY__4] = __4;
    bS12[KEY_5] = _5;    bS12[KEY__5] = __5;
    bS12[KEY_6] = _6;    bS12[KEY__6] = __6;
    bS12[KEY_7] = _7;    bS12[KEY__7] = __7;
    bS12[KEY_8] = _8;    bS12[KEY__8] = __8;
    bS12[KEY_9] = _9;    bS12[KEY__9] = __9;
    bS12[KEY_0] = _10;   bS12[KEY__0] = __10;

    bS12[KEY_Q]  = _on;  bS12[KEY__Q] = _off;
    bS12[KEY_W]  = _1on; bS12[KEY__W] = _1off;
    bS12[KEY_E]  = _2on; bS12[KEY__E] = _2off;

    bS12[KEY__A] = adcReadAll;
}
