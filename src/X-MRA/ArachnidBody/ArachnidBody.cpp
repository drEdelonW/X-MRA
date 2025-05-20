#include "ArachnidBody.hpp"

ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
    _legs(legs),
    _legCount(legCount),
    _legMaskLimit(0xFF >> (MAX_LEGS - _legCount)), //mask for legGroup limits
    _isArmed(false)
{
    setPatMask(0, LEGS_ALL);
    for( int i = 0;
        i < ((MAX_LEGS < _legCount)?
            MAX_LEGS : _legCount);
        i++ ) {
        _defaultPose[i] =
            _legs[i].legToBody(
                _legs[i].tipPosition(
                    deg(0.0f),
                    deg(0.0f),
                    deg(0.0f)
                )
            );
    }
}

void  ArachnidBody::setPatMask(int pattern, uint8_t mask) {
    _legMask[pattern] = mask & _legMaskLimit;
}
uint8_t ArachnidBody::getPatMask(int pattern) {
    return _legMask[pattern];
}

bool ArachnidBody::setOffs(Vector3D offs, int pattern) {
    for (int i = 0; i < _legCount; i++){
        if (_legMask[pattern] & (1 << i)) {
            if (
                _legs[i].checkTipPosition(
                    _legs[i].bodyToLeg(
                        _defaultPose[i] - offs
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

bool ArachnidBody::applyPose() {
    if (!_isArmed)
        return false;

    for (int i = 0; i < _legCount; i++){
        if (!_legs[i].applyPose()) {
            return false;
        }
    }
    return true;
}

bool ArachnidBody::ARM() {
    for(int i = 0; i < _legCount; i++) {
        _legs[i].engage();
    }
    return _isArmed = true;
};
void ArachnidBody::DISARM() {
    for(int i = 0; i < _legCount; i++) {
        _legs[i].release();
    }
    _isArmed = false;
};