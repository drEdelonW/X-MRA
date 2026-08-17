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
    _legToBody.reset(); // begin of forward matrix
    _legToBody *= M4x4::mxRotZ(rotation.asRadians()); {
        _legToBody *= M4x4::mxTrans(Vector3D{ offs, 0.f, 0.f }); {

            _bodyToLeg.reset(); // begin of reverse matrix
        } _bodyToLeg *= M4x4::mxTrans(Vector3D{ -offs, 0.f, 0.f });
    } _bodyToLeg *= M4x4::mxRotZ(-rotation.asRadians());
}

bool ArachnidLeg::checkJointAngles(legJn Ang) {
    return
        _Phalanx[ Coxa].jn.checkPose(Ang[ Coxa]) &&
        _Phalanx[Femur].jn.checkPose(Ang[Femur]) &&
        _Phalanx[Tibia].jn.checkPose(Ang[Tibia]);
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

