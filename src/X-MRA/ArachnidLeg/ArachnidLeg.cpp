#include "ArachnidLeg.hpp"

ArachnidLeg::ArachnidLeg(
    JointBase& coxaJn,
    JointBase& femurJn,
    JointBase& tibiaJn,
    Millimeters offs,
    Angle rotation
):  coxaJn_(coxaJn),
    femurJn_(femurJn),
    tibiaJn_(tibiaJn) {
    configMount(offs, rotation);
}

bool ArachnidLeg::checkJointAngles(Angle coxaAngle, Angle femurAngle, Angle tibiaAngle) {
    return
        coxaJn_.checkPose(coxaAngle) &&
        femurJn_.checkPose(femurAngle) &&
        tibiaJn_.checkPose(tibiaAngle);
}

bool ArachnidLeg::applyPose() {
    return
        tibiaJn_.applyPose() &&
        femurJn_.applyPose() &&
        coxaJn_.applyPose();
}

void ArachnidLeg::engage() {
    coxaJn_.engage();
    femurJn_.engage();
    tibiaJn_.engage();
}

void ArachnidLeg::release() {
    coxaJn_.release();
    femurJn_.release();
    tibiaJn_.release();
}

