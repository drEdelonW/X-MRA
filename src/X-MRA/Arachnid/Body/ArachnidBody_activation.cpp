#include "ArachnidBody.hpp"
#include <unistd.h>  //usleep
#include "terminal_tools.h"
#include "CLAMP.h"

float cPark =  90.f;    float cSafe =   0.f;    //
float fPark =  3.f;     float fSafe =  30.f;    // neg is rise ?
float tPark =  0.f;     float tSafe =  45.f;    float tRise = 10.f; // pos out
#define PHASE_DUR  us(1000000) /* 1 sec duration */


Vector3D ArmSQNC[] = {
    [0] = {cPark, fPark, tPark},  // Park Tibia
    [1] = {cPark, fPark, tSafe},  // Rise Tibia
    [2] = {cSafe, fPark, tSafe},
    [3] = {cSafe, fSafe, tRise},
    [4] = {cSafe, fSafe, tRise},
};


#include "CLAMP.h"
static pattern_t pattern = (MAX_LEGS - 1);
bool ArachnidBody::animAngDeg(
    Vector3D from,
    Vector3D to,
    MicroSeconds duration
) {
    setPatMask(pattern, LEGS_ALL);

    MicroSeconds tsEnd = microsNow() + duration;
#if 1
    MicroSeconds tsStart = microsNow();
    Vector3D delta = to - from;
    Vector3D curPose = from;
    for (; microsNow() < tsEnd;) {
        // curPose.print(); LOG("now[%lld]\n", now);
        PATTERN_LEG(pattern) {
            /**/ if (legIdx >= 4)   ClampMoreThen(&curPose.x, 30.f);
            else if (legIdx >= 2)   ClampMoreThen(&curPose.x, 60.f);

            legJn jAng = {deg(curPose.x), deg(curPose.y), deg(curPose.z)};
            if (!(legIdx & 1))      jAng[Coxa] = -jAng[Coxa];

            if (!_legs[legIdx].tryJointAngles(jAng)) {
                LOG("FALSE\n");
                return false;
            }
        }
        curPose = from + delta * ((microsNow() - tsStart) / duration);
    }
#else
        PATTERN_LEG(pattern) {
            /**/ if (legIdx >= 4)   ClampMoreThen(&to.x, 30.f);
            else if (legIdx >= 2)   ClampMoreThen(&to.x, 60.f);

            legJn jAng = {deg(to.x), deg(to.y), deg(to.z)};
            if (!(legIdx & 1))      jAng[Coxa] = -jAng[Coxa];

            if (!_legs[legIdx].tryJointAngles(jAng)) {
                LOG("FALSE\n");
                return false;
            }
        }
    while (tsEnd > microsNow()) {}
#endif
    return true;
}

/* ARM SQNC:
    raise tibias to safe zone
    move coxas to center (0)
    raise femurs to safe zone
    touch the ground (get XY coordinate to to get angle)
    use kinematic for rising
*/
bool ArachnidBody::ARM() {
    if (_isArmed) {
        LOG("ARMED\n");
        return true;
    }
    LOG("ARMING... ");

    setPatMask(pattern, LEGS_ALL);
    PATTERN_LEG(pattern){   // First action
        _legs[legIdx].engage();
    }

    animAngDeg(ArmSQNC[0], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[3], PHASE_DUR / 2);
    animAngDeg(ArmSQNC[3], ArmSQNC[4], PHASE_DUR);

#if 1
    AimSetAngle(deg(0), deg(0));
    trySetOffs(Vector3D{});
#endif

    LOG("DONE\n");
    return _isArmed = true;
};

/* ARM SQNC:
    use kinematic for lower
    raise tibias safe zone
    lower femurs to parking
    move coxas to parking
    lower tibias to parking
*/
void ArachnidBody::DISARM() {
    if (!_isArmed) {
        LOG("DISARMED\n");
        return;
    }
    LOG("DISARMING... ");
    setPatMask(pattern, LEGS_ALL);
#if 0
    PATTERN_LEG(pattern) {
        _legs[legIdx].tryJointAngles(
            deg(0), JFREEZE, JFREEZE
        );
    }

    for (int ang = 0; ang < 90; ang++) {
        PATTERN_LEG(pattern) {
            _legs[legIdx].tryJointAngles(
                JFREEZE, deg(-ang), deg(-(ang * 0.8f))
            );
        }
        usleep(1000);
    }
#endif
    AimSetAngle(deg(0.f), deg(0.f));

    trySetOffs(Vector3D{});
    usleep(1000000);
#if 0
#endif
    animAngDeg(ArmSQNC[4], ArmSQNC[3], PHASE_DUR);
    animAngDeg(ArmSQNC[3], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[0], PHASE_DUR);
    usleep(1000000);

    PATTERN_LEG(pattern) {   // Last action
        _legs[legIdx].release();
    }
    _isArmed = false;
    LOG("DONE\n");
};