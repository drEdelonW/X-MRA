#include "ArachnidLeg.hpp"
#include <cmath>

ArachnidLeg::ArachnidLeg(
    JointBase& coxaJn,
    JointBase& femurJn,
    JointBase& tibiaJn
):  coxaJn_(coxaJn),
    femurJn_(femurJn),
    tibiaJn_(tibiaJn) {
}

bool ArachnidLeg::checkJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle) {
    return
        coxaJn_.checkPose(coxaAngle) &&
        femurJn_.checkPose(femurAngle) &&
        tibiaJn_.checkPose(tibiaAngle);
}
bool ArachnidLeg::checkTipPosition(Millimeters x,Millimeters y, Millimeters z) {
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
bool ArachnidLeg::checkTipPosition(Vector3D pos){
    return checkTipPosition(pos.x, pos.y, pos.z);
}

Vector3D ArachnidLeg::tipPosition(Angle coxaAng, Angle femurAng, Angle tibiaAng) {
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

bool ArachnidLeg::applyPose() {
    return
        coxaJn_.applyPose() &&
        femurJn_.applyPose() &&
        tibiaJn_.applyPose();
}


void ArachnidLeg::activate() {
    coxaJn_.engage();
    femurJn_.engage();
    tibiaJn_.engage();
}
void ArachnidLeg::deactivate() {
    coxaJn_.release();
    femurJn_.release();
    tibiaJn_.release();
}

