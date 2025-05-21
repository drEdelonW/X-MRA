#include "ArachnidBody.hpp"

bool ArachnidBody::setOffs(Vector3D offs, int pattern) {
    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            if ( _legs[i].checkTipPosBodySpace(_legExtras[i].defaultPose - offs)) {
                continue;
            } else {
                _lastLegError = i;
                return false;
            }
        }
    }
    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            _legExtras[i].deltaMatrix.reset();
            _legExtras[i].deltaMatrix *= Matrix4x4::createTranslation(-offs);
            _legExtras[i].currentPose = _legExtras[i].defaultPose - offs;
        }
    }
    return true;
}
bool ArachnidBody::trySetOffs(Vector3D offs, int pattern) {
    return
        setOffs(offs, pattern) &&
        applyPose(pattern);
}


bool ArachnidBody::addOffs(Vector3D offs, int pattern) {
    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            if ( _legs[i].checkTipPosBodySpace( _legExtras[i].currentPose - offs)) {
                continue;
            } else {
                _lastLegError = i;
                return false;
            }
        }
    }
    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            _legExtras[i].deltaMatrix *= Matrix4x4::createTranslation(-offs);
            _legExtras[i].currentPose = _legExtras[i].deltaMatrix.applyTransform( _legExtras[i].defaultPose );
        }
    }
    return true;
}
bool ArachnidBody::tryAddOffs(Vector3D offs, int pattern) {
    return
        addOffs(offs, pattern) &&
        applyPose(pattern);
}

// bool ArachnidBody::setRotationOX(Angle angle, int pattern = 0);
// bool ArachnidBody::setRotationOY(Angle angle, int pattern = 0);
// bool ArachnidBody::setRotationOZ(Angle angle, int pattern = 0);