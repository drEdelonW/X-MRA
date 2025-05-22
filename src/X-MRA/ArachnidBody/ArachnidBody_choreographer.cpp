#include "ArachnidBody.hpp"

void  ArachnidBody::setPatMask(int pattern, uint8_t mask) {
    _legPattMask[pattern] = mask & _legMaskLimit;
}
uint8_t ArachnidBody::getPatMask(int pattern) {
    return _legPattMask[pattern];
}

bool ArachnidBody::applyPose(int pattern) {
    if (!_isArmed) {
        _lastLegError = MAX_LEGS; return false;
    }
    PATTERN_LEG {
        _legExtras[i].currentPose =
            _legPattMatrix[pattern].applyTransform(
                _legExtras[i].defaultPose
            );
    }

    PATTERN_LEG {
        if ( !_legs[i].checkTipPosBodySpace(_legExtras[i].currentPose))
            { LEG_ERROR; }
    }

    PATTERN_LEG {
        if ( !_legs[i].applyPose())
            { LEG_ERROR; }
    }
    LEG_ERROR_OK;
}
