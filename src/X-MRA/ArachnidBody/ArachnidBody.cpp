#include "ArachnidBody.hpp"

ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
    _legs(legs),
    _legCount(legCount),
    _isArmed(false),
    _legMaskLimit(0xFF >> (MAX_LEGS - _legCount)) //mask for legGroup limits
{
    setPatMask(0, LEGS_ALL);
    // setPatMask(0, TRIPOD_A);
    for( int i = 0;
        i < ((MAX_LEGS < _legCount)?
            MAX_LEGS : _legCount);
        i++ ) {
        _legExtras[i].currentPose =
        _legExtras[i].defaultPose =
            _legs[i].tipPosBodySpace( deg(0.0f), deg(0.0f), deg(0.0f));
        _legExtras[i].deltaMatrix.reset();
    }
}
