#include "ArachnidBody.hpp"

bool ArachnidBody::setOffs(Vector3D offs, int pattern) {
    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            if (
                _legs[i].checkTipPosition(
                    _legs[i].bodyToLeg(
                        _legExtras[i].defaultPose - offs
                    )
                )
            ) {
                continue;
            } else {
                return false;
            }
        }
    }
    return true;
}

// bool ArachnidBody::addOffs(Vector3D offs, int pattern = 0);
// bool ArachnidBody::setRotationOX(Angle angle, int pattern = 0);
// bool ArachnidBody::setRotationOY(Angle angle, int pattern = 0);
// bool ArachnidBody::setRotationOZ(Angle angle, int pattern = 0);