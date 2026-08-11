#include "dpad.h"

static void _begin();
dPadBind bL = { [KEY_UNKNOWN] = _begin };

#include "common_tools.h"
#include <unistd.h>  //usleep

#include "X-MRA.hpp"
#include "Vector3d.hpp"

#if 0
#define NUM_LEGS    (6)
#define LEG_J_ALL(v1, v2, v3)  \
    for(int idxLeg = 0; idxLeg < NUM_LEGS; idxLeg++) { \
        if (XMRA._legs[idxLeg].checkJointAngles(deg(v1),deg(v2),deg(v3))) \
            XMRA._legs[idxLeg].applyPose(); \
    }

#define LEG_P_ALL(v1, v2, v3)  \
    for(int idxLeg = 0; idxLeg < NUM_LEGS; idxLeg++) { \
        if (XMRA._legs[idxLeg].checkTipPosition(v1,v2,v3)) \
            XMRA._legs[idxLeg].applyPose(); \
    }

#define LEG_PV_ALL(v)  \
    for(int idxLeg = 0; idxLeg < NUM_LEGS; idxLeg++) { \
        if (XMRA._legs[idxLeg].checkTipPosition(v)) \
            XMRA._legs[idxLeg].applyPose(); \
    }

#define LEG_PON_ALL()  {for(int idxLeg = 0; idxLeg < NUM_LEGS; idxLeg++) { XMRA._legs[idxLeg].engage(); }}
#define LEG_POFF_ALL() {for(int idxLeg = 0; idxLeg < NUM_LEGS; idxLeg++) { XMRA._legs[idxLeg].release(); }}
#endif

static void _dArm()  {
    if (XMRA.isArmed()) return;
    LOG("ON\n");
    beep(true); {
        PowerAllow(true);
        XMRA.ARM();
    } beep(false);
}
static void _dDisarm() {
    LOG("OFF\n");
    beep(true); {
        XMRA.DISARM();
        PowerAllow(false);
    } beep(false);
}

Millimeters mStep = 1.f;

static void _1()  { mStep =  0.1f; }
static void _2()  { mStep =  1.f;  }
static void _3()  { mStep = 10.f;  }

const Millimeters coxaLength  = mm( 27.f);
const Millimeters femurLength = mm( 85.1f);
const Millimeters tibiaLength = mm(144.23f);

const Millimeters tibiaX      = mm( 27.33f);
const Millimeters tibiaZ      = mm(141.6f);

#if 0
#   define DSTx    (coxaLength + femurLength + tibiaX)
#   define DSTy    (0.f)
#   define DSTz    (-tibiaZ)
#else
#   define DSTx    (0.f)
#   define DSTy    (0.f)
#   define DSTz    (0.f)
#endif

Vector3D _curPose = {DSTx, DSTy, DSTz};

static void _xPos() { _curPose.x += mStep;  XMRA.trySetOffs(_curPose); }
static void _xNeg() { _curPose.x -= mStep;  XMRA.trySetOffs(_curPose); }

static void _yPos() { _curPose.y -= mStep;  XMRA.trySetOffs(_curPose); }
static void _yNeg() { _curPose.y += mStep;  XMRA.trySetOffs(_curPose); }

static void _zPos() { _curPose.z += mStep;  XMRA.trySetOffs(_curPose); }
static void _zNeg() { _curPose.z -= mStep;  XMRA.trySetOffs(_curPose); }

static void _goHome()  { _curPose = {DSTx, DSTy, DSTz};  XMRA.trySetOffs(_curPose); }

static void _info() {
    for (int idxLeg = 0; idxLeg < 6; idxLeg++){
        LOG("[%d] ", idxLeg);
        LOG("legSpace:");       XMRA._legs[idxLeg].tipPosLegSpace(deg(0), deg(0), deg(0)).print();
        LOG("\t bodySpace:");   XMRA._legs[idxLeg].tipPosBodySpace(deg(0), deg(0), deg(0)).print();
        LOG("\n");
    }
}

static void _begin() {
    LOG(CLEAR_SCREEN
        "Leg dPad begin\n"
        "</>        - {x}\n"
        "^/v        - {y}\n"
        "Sh + ^/v   - {z}\n"
        "PgUp/PgDown    - Arm/Disarm\n"
        "Home           - GoHome\n"
        "i  - Info\n"
    );

    bL[KEY_ESCAPE] = dPadQuit;

    bL[KEY_I] = _info;

    bL[KEY_LEFT]    = _yPos;    bL[KEY_RIGHT]   = _yNeg;
    bL[KEY_UP]      = _xPos;    bL[KEY_DOWN]    = _xNeg;
    bL[KEY__UP]     = _zPos;    bL[KEY__DOWN]   = _zNeg;

    bL[KEY_PAGE_UP]   = _dArm;  bL[KEY_PAGE_DOWN] = _dDisarm;
    bL[KEY_HOME]    = _goHome;

    bL[KEY_1] = _1; bL[KEY_2] = _2; bL[KEY_3] = _3;
}

