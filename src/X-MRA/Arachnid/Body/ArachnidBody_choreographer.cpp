#include "ArachnidBody.hpp"

void  ArachnidBody::setPatMask(int pattern, LegMask mask) {
    _legPattMask[pattern] = (LegMask)(mask & _legMaskLimit);
}

LegMask ArachnidBody::getPatMask(int pattern) {
    return _legPattMask[pattern];
}

bool ArachnidBody::applyPose(int pattern) {
    if (!_isArmed) {
        _lastLegError = MAX_LEGS;
        return false;
    }

    PATTERN_LEG{
        _legExtras[i].currentPose =
            (_legPattMatrix[pattern] * _ctrlMatrix).applyTransform(
                _legExtras[i].defaultPose
            );
    };

    PATTERN_LEG{
        if (!_legs[i].checkTipPosBodySpace(_legExtras[i].currentPose))
            LEG_ERROR;
    };

    PATTERN_LEG{
        if (!_legs[i].applyPose())
            LEG_ERROR;
    };
    LEG_ERROR_OK;
}

void ArachnidBody::getMatrix(Matrix4x4_p mx, int pattern) {
    if (pattern < MAX_LEGS)
        *mx = _legPattMatrix[pattern];
}
void ArachnidBody::setMatrix(Matrix4x4_p mx, int pattern) {
    if (pattern < MAX_LEGS)
        _legPattMatrix[pattern] = *mx;
}
