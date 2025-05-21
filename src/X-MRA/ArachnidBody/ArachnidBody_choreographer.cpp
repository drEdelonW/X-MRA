#include "ArachnidBody.hpp"

void  ArachnidBody::setPatMask(int pattern, uint8_t mask) {
    _legMask[pattern] = mask & _legMaskLimit;
}
uint8_t ArachnidBody::getPatMask(int pattern) {
    return _legMask[pattern];
}

bool ArachnidBody::applyPose(int pattern) {
    if (!_isArmed)
        return false;

    for (int i = 0; i < _legCount; i++){
        if (_maskCheck(pattern, i)) {
            if (!_legs[i].applyPose()) {
                _lastLegError = i;
                return false;
            }
        }
    }
    return true;
}
