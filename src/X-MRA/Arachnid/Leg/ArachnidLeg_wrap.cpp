#include "ArachnidLeg.hpp"

bool ArachnidLeg::tryJointAngles(Angle coxaAng, Angle femurAng, Angle tibiaAng) {
    return
        checkJointAngles(coxaAng, femurAng, tibiaAng) &&    // check before
        applyPose();                                        // before apply
}


bool ArachnidLeg::checkTipPosLegSpace(Vector3D pos) {
    return _checkTipPosLegSpace(pos);
}

bool ArachnidLeg::checkTipPosBodySpace(Vector3D bodyPos) {
    return _checkTipPosLegSpace(bodyToLeg(bodyPos));
}

Vector3D ArachnidLeg::tipPosBodySpace(Angle coxaAng, Angle femurAng, Angle tibiaAng) {
    return legToBody(
        tipPosLegSpace(
            coxaAng,
            femurAng,
            tibiaAng
        )
    );
}

Vector3D ArachnidLeg::bodyToLeg(Vector3D bodyPos) {
    return _bodyToLeg.applyTransform(bodyPos);
}

Vector3D ArachnidLeg::legToBody(Vector3D legPos) {
    return _legToBody.applyTransform(legPos);
}

