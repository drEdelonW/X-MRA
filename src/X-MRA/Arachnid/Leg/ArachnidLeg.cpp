#include "ArachnidLeg.hpp"

ArachnidLeg::ArachnidLeg(
    Phalanx_p   Phalanx,
    Millimeters offs,
    Angle       rotation
):
    _Phalanx{
         [Coxa] = Phalanx[Coxa],
        [Femur] = Phalanx[Femur],
        [Tibia] = Phalanx[Tibia]
    }
{
    _configMount(offs, rotation);
}

bool ArachnidLeg::checkJointAngles(
    Angle  coxaAngle,
    Angle femurAngle,
    Angle tibiaAngle
) {
    return
         _Phalanx[Coxa].jn.checkPose( coxaAngle) &&
        _Phalanx[Femur].jn.checkPose(femurAngle) &&
        _Phalanx[Tibia].jn.checkPose(tibiaAngle);
}

Angle ArachnidLeg::getJointAngles(LegJoint jName){
    return
        (jName < PhalNum)?
            _Phalanx[jName].jn.getAngle() : rad(NAN);
}

bool ArachnidLeg::applyPose() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        if (!_Phalanx[jIdx].jn.applyPose())
            return false;
    return true;
}

void ArachnidLeg::engage() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        _Phalanx[jIdx].jn.engage();
}

void ArachnidLeg::release() {
    for (int jIdx = 0; jIdx < PhalNum; jIdx++)
        _Phalanx[jIdx].jn.release();
}

