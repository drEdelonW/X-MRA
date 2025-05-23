#include "ArachnidBody.hpp"

ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
    _legs(legs),
    _legCount(legCount),
    _isArmed(false),
    _legMaskLimit(0xFF >> (MAX_LEGS - _legCount)), //mask for legGroup limits
    _lastLegError(MAX_LEGS),
    _azimuth(deg(0)),
    _elevation(deg(0))
{
    int8_t pattern = 0;
    setPatMask(pattern, LEGS_ALL);
    PATTERN_LEG {
        _legExtras[i].defaultPose =
            _legs[i].tipPosBodySpace( deg(0.0f), deg(0.0f), deg(0.0f));
    }
}
