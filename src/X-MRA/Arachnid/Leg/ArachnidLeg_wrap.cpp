#include "ArachnidLeg.hpp"

bool ArachnidLeg::tryJointAngles(legJn Ang) {
    return
        checkJointAngles(Ang) &&    // check before
        applyPose();                // before apply
}

Vector3D ArachnidLeg::legToBody(Vector3D legPos)                { return _legToBody.applyTransform(legPos); }
Vector3D ArachnidLeg::bodyToLeg(Vector3D bodyPos)               { return _bodyToLeg.applyTransform(bodyPos); }
Vector3D ArachnidLeg::tipPosBodySpace(legJn Ang)                { return legToBody( tipPosLegSpace(Ang) ); }
bool     ArachnidLeg::checkTipPosBodySpace(Vector3D bodyPos)    { return _checkTipPosLegSpace( bodyToLeg(bodyPos) ); }
bool     ArachnidLeg::checkTipPosLegSpace(Vector3D pos)         { return _checkTipPosLegSpace(pos); }
