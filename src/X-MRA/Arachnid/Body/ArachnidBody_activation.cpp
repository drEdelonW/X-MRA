#include "ArachnidBody.hpp"
#include <unistd.h>  //usleep
#include "terminal_tools.h"
#include "CLAMP.h"

// Vector3D cPark = V0;
float cPark = 85.f;
float cSafe = 0.f;

float fPark = -125.f;
float fSafe = -90.f;

float tPark = -80.f;
float tSafe = -30.f;

#define PHASE_DUR  us(1000000) /* 1 sec duration */

Vector3D ArmSQNC[] = {
    {cPark, fPark, tPark},
    {cPark, fPark, tSafe},
    {cSafe, fPark, tSafe},
    {cSafe, fSafe, tSafe},
};

bool ArachnidBody::isArmed() {
    return _isArmed;
}

bool ArachnidBody::animAngDeg(
    Vector3D from,
    Vector3D to,
    MicroSeconds duration
) {
    int8_t pattern = 0;
    MicroSeconds now = microsNow();
    MicroSeconds tsStart = now;
    MicroSeconds tsEnd = now + duration;
    Vector3D delta = to - from;
    setPatMask(pattern, LEGS_ALL);
    for (
        Vector3D curPose = from;
        (now = microsNow()) < tsEnd;
        curPose += delta *
            (
                (now - tsStart) /
                    duration
            )

        ) {
        // curPose.print(); LOG("now[%lld]\n", now);
        PATTERN_LEG{
            /**/ if (legIdx >= 4)     ClampMoreThen(&curPose.x, 30.f);
            else if (legIdx >= 2)     ClampMoreThen(&curPose.x, 60.f);

            if (!_legs[legIdx].tryJointAngles(
                    (legIdx % 2) ?
                        deg(curPose.x) : -deg(curPose.x),
                    deg(curPose.y),
                    deg(curPose.z)
                )
            ) {
                LOG("FALSE\n");
                return false;
            }
        }
    }
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

    int8_t pattern = 0;
    setPatMask(pattern, LEGS_ALL);
    PATTERN_LEG{   // First action
        _legs[legIdx].engage();
    }

    animAngDeg(ArmSQNC[0], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[3], PHASE_DUR / 2);
    animAngDeg(ArmSQNC[3], ArmSQNC[4], PHASE_DUR);

    AimSetAngle(deg(0), deg(0));
    trySetOffs(Vector3D{});

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
    int8_t pattern = 0;
    setPatMask(pattern, LEGS_ALL);
#if 0
    PATTERN_LEG{
        _legs[legIdx].tryJointAngles(
            deg(0), JFREEZE, JFREEZE
        );
    }

    for (int ang = 0; ang < 90; ang++) {
        PATTERN_LEG{
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
    animAngDeg(ArmSQNC[4], ArmSQNC[3], PHASE_DUR);
    animAngDeg(ArmSQNC[3], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[0], PHASE_DUR);
    usleep(1000000);

    PATTERN_LEG{   // Last action
        _legs[legIdx].release();
    }
    _isArmed = false;
    LOG("DONE\n");
};