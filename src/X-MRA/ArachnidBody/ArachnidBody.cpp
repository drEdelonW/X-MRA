#include "ArachnidBody.hpp"

ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
    _legs(legs),
    _legCount(legCount),
    _legMaskLimit(0xFF >> (MAX_LEGS - _legCount)), //mask for legGroup limits
    _isArmed(false)
{
    _legMask[0] = 0xFF;
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

bool ArachnidBody::applyPose() { return false; }

bool ArachnidBody::ARM() {
    for(int i = 0; i < _legCount; i++) {
        _legs[i].engage();
    }
    return _isArmed;
};
void ArachnidBody::DISARM() {
    for(int i = 0; i < _legCount; i++) {
        _legs[i].release();
    }
    _isArmed = false;
};