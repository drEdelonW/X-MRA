#include "ArachnidLeg.hpp"
#include <cmath>

void ArachnidLeg::configMount(Millimeters offset, Angle yaw) {
    _legToBody.reset();
    _legToBody *= Matrix4x4::createRotationZ(yaw.asRadians());
    _legToBody *= Matrix4x4::createTranslation({offset, 0.0f, 0.0f});

    _bodyToLeg.reset();
    _bodyToLeg *= Matrix4x4::createTranslation({-offset, 0.0f, 0.0f});
    _bodyToLeg *= Matrix4x4::createRotationZ(-yaw.asRadians());
}

bool ArachnidLeg::_checkTipPosLegSpace(Millimeters x,Millimeters y, Millimeters z) {
    float angleCoxa = atan2(y, x);

    float planarX = sqrtf((x*x) + (y*y)) - coxaLength_;
    float planarZ = z;

    float dist = sqrtf((planarX*planarX) + (planarZ*planarZ));
    float maxReach = femurLength_ + tibiaLength_;
    float minReach = fabsf(femurLength_ - tibiaLength_);

    if ((dist < minReach) || dist > maxReach)
        return false;                           // unreachable

    float angleToTarget = atan2f(planarZ, planarX);

    float a = femurLength_;
    float b = tibiaLength_;
    float c = dist;

    /* correct law-of-cosines */
    float angleFemurRel = acosf((a*a + c*c - b*b) / (2.0f*a*c));
    float angleKnee     = acosf((a*a + b*b - c*c) / (2.0f*a*b));

    float angleFemur = angleToTarget + angleFemurRel;
    float angleTibia = M_PI - angleKnee;

    return
        checkJointAngles(
            rad(angleCoxa),
            rad(-angleFemur),
            rad(-angleTibia) + deg(90 - 10.9)
        );
}

Vector3D ArachnidLeg::tipPosLegSpace(Angle coxaAng, Angle femurAng, Angle tibiaAng) {
    float aC = coxaAng.asRadians();
    float aF = femurAng.asRadians();
    float aT = (tibiaAng - deg(90.0f - 10.9f)).asRadians();

    float knee = aF + aT;               // femur-tibia plane

    float rx = coxaLength_ +
        femurLength_ * cosf(aF) +
        tibiaLength_ * cosf(knee);    // projection in Coxa plane

    float rz =
        femurLength_ * sinf(aF) +
        tibiaLength_ * sinf(knee);

    float cx = cosf(aC);
    float sx = sinf(aC);

    return { rx * cx, rx * sx, rz };
}
