#include "ArachnidLeg.hpp"
#include <cmath>

ArachnidLeg::ArachnidLeg(
    JointBase& coxaJn,
    JointBase& femurJn,
    JointBase& tibiaJn
    ):
    coxaJn_(coxaJn),
    femurJn_(femurJn),
    tibiaJn_(tibiaJn) {

}

bool ArachnidLeg::checkJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle) {
    return
        coxaJn_.checkPose(coxaAngle) &&
        femurJn_.checkPose(femurAngle) &&
        tibiaJn_.checkPose(tibiaAngle);
}
bool ArachnidLeg::checkTipPosition(Millimeters x, Millimeters y, Millimeters z) {
    // Coxa angle from projection onto XY-plane
    float angleCoxa = atan2(y, x); // V

    // Shift to local femur-tibia plane
    float planarX = sqrt(x * x + y * y) - coxaLength_;
    float planarZ = z; // downward is positive

    // Distance from femur joint to target
    float dist = sqrt(planarX * planarX + planarZ * planarZ);

    float maxReach = femurLength_ + tibiaLength_;
    float minReach = fabsf(femurLength_ - tibiaLength_); // folded inward

    if (dist < minReach + 1e-3f || dist > maxReach - 1e-3f) {
        return false; // unreachable
    }

    // dist = clamp(dist, 1e-3f, femurLength_ + tibiaLength_ - 1e-3f);

    float angleToTarget = atan2(planarZ, planarX);

    float a = femurLength_;
    float b = tibiaLength_;
    float c = dist;

    // Use law of cosines
    float angleA = acos(clamp((b*b + c*c - a*a) / (2*b*c), -1.0f, 1.0f)); // femur
    float angleB = acos(clamp((a*a + b*b - c*c) / (2*a*b), -1.0f, 1.0f)); // tibia

    float angleFemur = angleToTarget + angleA;
    float angleTibia = M_PI - angleB;

    return
        checkJointAngles(
            rad(angleCoxa),
            rad(-angleFemur),
            rad(-angleTibia) + deg(90 - 10.9)
        );
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

