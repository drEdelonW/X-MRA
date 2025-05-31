#include "ArachnidBody.hpp"
#include <unistd.h>  //usleep
#include "terminal_tools.h"

// Vector3D cPark = {0.0, 0.0, 0.0};
float cPark = 85.0;
float cSafe =  0.0;

float fPark = -125.0;
float fSafe = -90.0;

float tPark = -80.0;
float tSafe = -30.0;

#define PHASE_DUR  (1000000)

Vector3D ArmSQNC [] = {
    {cPark, fPark, tPark},
    {cPark, fPark, tSafe},
    {cSafe, fPark, tSafe},
    {cSafe, fSafe, tSafe},
};

bool ArachnidBody::isArmed() {
    return _isArmed;
}

bool ArachnidBody::animAngDeg(Vector3D from ,Vector3D to, MicroSeconds duration) {
    int8_t pattern = 0;
    MicroSeconds tsStart = microsNow();
    MicroSeconds tsEnd = tsStart + duration;
    MicroSeconds now ;
    Vector3D delta = to - from;
    setPatMask(pattern, LEGS_ALL);
    for (
        Vector3D curPose = from;
        (now = microsNow()) < tsEnd;
        curPose = from + (delta * (now - tsStart) / (duration * 1.0f))
    ) {
        // curPose.print(); LOG("now[%lld]\n", now);
        PATTERN_LEG {

            float coxa = curPose.x * ((i%2)? 1.0f : -1.0f);
            if ((i > 1) &&
                (curPose.x > 60.0f)
            ) {
                coxa = (i%2)? 60.0f : -60.0f;
            }
            if ((i > 3) &&
                (curPose.x > 30.0f)
            ) {
                coxa = (i%2)? 30.0f : -30.0f;
            }
            if (!_legs[i].tryJointAngles(
                    deg(coxa),
                    deg(curPose.y),
                    deg(curPose.z)
                )) {
                    LOG("FALSE\n")
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
    PATTERN_LEG {   // First action
        _legs[i].engage();
    }

    animAngDeg(ArmSQNC[0], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[3], PHASE_DUR / 2);
    animAngDeg(ArmSQNC[3], ArmSQNC[4], PHASE_DUR);

    AimSetAngle(deg(0), deg(0));
    trySetOffs(Vector3D{0.0f, 0.0f, 0.0f});

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

    // PATTERN_LEG {
    //     _legs[i].tryJointAngles(deg(0), JFREEZE, JFREEZE);
    // }

    // for (int ang = 0; ang < 90; ang++){
    //     PATTERN_LEG {
    //         _legs[i].tryJointAngles( JFREEZE, deg(-ang), deg(-(ang * 0.8f)));
    //     }
    //     usleep(1000);
    // }
    AimSetAngle(deg(0), deg(0));

    trySetOffs(Vector3D{0.0f, 0.0f, 0.0f});
    usleep(1000000);
    animAngDeg(ArmSQNC[4], ArmSQNC[3], PHASE_DUR);
    animAngDeg(ArmSQNC[3], ArmSQNC[2], PHASE_DUR);
    animAngDeg(ArmSQNC[2], ArmSQNC[1], PHASE_DUR);
    animAngDeg(ArmSQNC[1], ArmSQNC[0], PHASE_DUR);
    usleep(1000000);

    PATTERN_LEG {   // Last action
        _legs[i].release();
    }
    _isArmed = false;
    LOG("DONE\n");
};