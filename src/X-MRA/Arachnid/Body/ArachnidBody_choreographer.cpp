#include "ArachnidBody.hpp"

void  ArachnidBody::setPatMask(pattern_t pattern, LegMask mask) {
    _legPattMask[pattern] = (LegMask)(mask & _legMaskLimit);
}

LegMask ArachnidBody::getPatMask(pattern_t pattern) {
    return _legPattMask[pattern];
}

bool ArachnidBody::applyPose(pattern_t pattern) {
    if (!_isArmed) {
        _lastLegError = MAX_LEGS;
        return false;
    }
    if (pattern < MAX_LEGS) {
        PATTERN_LEG(pattern) {
            _legExtras[legIdx].currentPose =
                (_legPattMatrix[pattern] * _ctrlMatrix).applyTransform(
                    _legExtras[legIdx].defaultPose
                );
        };

        PATTERN_LEG(pattern) {
            // if (!_legs[legIdx].checkTipPosLegSpace(
            if (!_legs[legIdx].checkTipPosBodySpace(
                    _legExtras[legIdx].currentPose
                )
            )   LEG_ERROR;
        };

        PATTERN_LEG(pattern) {
            if (!_legs[legIdx].applyPose())
                LEG_ERROR;
        };
        LEG_ERROR_OK;
    }
    return false;
}

void ArachnidBody::getMatrix(Matrix4x4_p mx, pattern_t pattern) {
    if (pattern < MAX_LEGS)
        *mx = _legPattMatrix[pattern];
}
void ArachnidBody::setMatrix(Matrix4x4_p mx, pattern_t pattern) {
    if (pattern < MAX_LEGS)
        _legPattMatrix[pattern] = *mx;
}
