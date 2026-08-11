#include "ArachnidLeg.hpp"

ArachnidLeg::ArachnidLeg(
    JointBase*  Jn,
    // ServoJoint* Jn,
    Millimeters offs,
    Angle       rotation
):
    _jn{ [Coxa] = &Jn[Coxa],
        [Femur] = &Jn[Femur],
        [Tibia] = &Jn[Tibia]}
{
    configMount(offs, rotation);
}
ArachnidLeg::ArachnidLeg(
    // JointBase*  Jn,
    ServoJoint* Jn,
    Millimeters offs,
    Angle       rotation
):
    _jn{ [Coxa] = &Jn[Coxa],
        [Femur] = &Jn[Femur],
        [Tibia] = &Jn[Tibia]}
{
    configMount(offs, rotation);
}

bool ArachnidLeg::checkJointAngles(
    Angle coxaAngle,
    Angle femurAngle,
    Angle tibiaAngle
) {
    return
        _jn[Coxa]->checkPose(coxaAngle) &&
        _jn[Femur]->checkPose(femurAngle) &&
        _jn[Tibia]->checkPose(tibiaAngle);
}

Angle ArachnidLeg::getJointAngles(LegJoint jName){
    return (jName < PhalNum)?
        _jn[jName]->getAngle() : rad(NAN);
}

bool ArachnidLeg::applyPose() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        if (!_jn[jIdx]->applyPose())
            return false;
    return true;
}

void ArachnidLeg::engage() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        _jn[jIdx]->engage();
}

void ArachnidLeg::release() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        _jn[jIdx]->release();
}

