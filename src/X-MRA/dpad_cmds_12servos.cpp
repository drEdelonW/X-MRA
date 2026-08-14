#include "dpad.h"

static void _begin();
dPadBind bS12 = { [KEY_UNKNOWN] = _begin };

// #include "common_tools.h"

#include "X-MRA.hpp"
#include "WS2812_Spi.hpp"
#define WS_LED_COUNT (7)
Ws2812Spi RING7(WS_LED_COUNT);
#define servoNum (6*3)

#include "PS_FreeNove.hpp"  // sFreeNove
extern sFreeNove Servo[];

uint8_t lPatt = 0b111111;
uint8_t jPatt = 0x07;
inline bool isActive(int i) {
     return
        (lPatt & (1 << (i / 3))) &&
        (jPatt & (1 << (i % 3)));
}

int map[7] = {2, 4, 1, 5, 0, 6, 3};
bool isAll = false;
static float _curAngle = 2.f;
void allSv(float ang, float dAng = 0.f) {
    LOG("\r[%.2f]    ", (_curAngle = ang));
    for(int idxSv = 0; idxSv < servoNum; idxSv++)
        if (isAll ||
            (isActive(idxSv))
            )   if (dAng != 0.f) {
                    Servo[idxSv].setAngle(
                        Servo[idxSv].getAngle() + deg(dAng)
                    );
                } else {
                    Servo[idxSv].setAngle(deg(ang));
                }

    for (int i = 0; i < RING7.getNumLeds(); i++) {
        RING7.setPixel(
            map[i],
            RGB(
                ((jPatt & (1 << Coxa)) != 0),
                ((jPatt & (1 << Femur)) != 0),
                ((jPatt & (1 << Tibia)) != 0)
            ) * ((lPatt & (1 << i)) != 0)
        );
    }
    RING7.show();
}

static void _left()  { allSv(0.f, -_curAngle); }
static void _right() { allSv(0.f, _curAngle); }

static void  _1()   { lPatt = (1 << 0); allSv(0); }
static void  _2()   { lPatt = (1 << 1); allSv(0); }
static void  _3()   { lPatt = (1 << 2); allSv(0); }
static void  _4()   { lPatt = (1 << 3); allSv(0); }
static void  _5()   { lPatt = (1 << 4); allSv(0); }
static void  _6()   { lPatt = (1 << 5); allSv(0); }

static void __1()   { lPatt ^= (1 << 0); allSv(0); }
static void __2()   { lPatt ^= (1 << 1); allSv(0); }
static void __3()   { lPatt ^= (1 << 2); allSv(0); }
static void __4()   { lPatt ^= (1 << 3); allSv(0); }
static void __5()   { lPatt ^= (1 << 4); allSv(0); }
static void __6()   { lPatt ^= (1 << 5); allSv(0); }

static void __7()   { jPatt ^= (1 << 0); allSv(0); }
static void __8()   { jPatt ^= (1 << 1); allSv(0); }
static void __9()   { jPatt ^= (1 << 2); allSv(0); }


static void _on()  { LOG("ON\n");  PowerAllow(true);  }
static void _off() { LOG("OFF\n"); PowerAllow(false); }

static void _1on()  { LOG("1 ON\n");  Servo[1].enable();  }
static void _1off() { LOG("1 OFF\n"); Servo[1].disable(); }

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
        // "0..9 +/-Sh - setAng(0..9)\n"
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

    bS12[KEY__7] = __7;
    bS12[KEY__8] = __8;
    bS12[KEY__9] = __9;

    // bS12[KEY_0] = _0;    bS12[KEY__0] = __0;

    bS12[KEY__Q] = _on;  bS12[KEY_Q] = _off;
    bS12[KEY__W] = _1on; bS12[KEY_W] = _1off;
    bS12[KEY__E] = _2on; bS12[KEY_E] = _2off;

    bS12[KEY__A] = adcReadAll;
}
