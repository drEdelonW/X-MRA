#include "ArachnidBody.hpp"
#include <unistd.h>  //usleep

ArachnidBody::ArachnidBody(ArachnidLeg* legs, size_t legCount):
    _legs(legs),
    _legCount(legCount),
    _legMaskLimit(0xFF >> (MAX_LEGS - _legCount)), //mask for legGroup limits
    _isArmed(false)
{
    setPatMask(0, LEGS_ALL);
    // setPatMask(0, TRIPOD_A);
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

bool ArachnidBody::isArmed() {
    return _isArmed;
}


bool ArachnidBody::ARM() {
    if (_isArmed)
        return true;

    for(int i = 0; i < _legCount; i++) {
        _legs[i].engage();
        _legs[i].checkTipPosition(
            _legs[i].bodyToLeg(
                _defaultPose[i]
            )
        );
    }
    return _isArmed = true;
};
void ArachnidBody::DISARM() {
    if (!_isArmed)
        return;

    for(int i = 0; i < _legCount; i++) {
        _legs[i].checkJointAngles(deg(0),deg(NAN),deg(NAN));
        _legs[i].applyPose();
        usleep(500);
    }

    for (int ang = 0; ang < 90; ang++){
        for(int i = 0; i < _legCount; i++) {
            _legs[i].checkJointAngles(deg(NAN),deg(-ang),deg(-(ang * 0.8f)));
            _legs[i].applyPose();
            usleep(1000);
        }
    }
    usleep(1000000);
    // for(int i = 0; i < _legCount; i++) {
    //     _legs[i].checkJointAngles(deg(20),deg(NAN),deg(NAN));
    //     _legs[i].applyPose();
    //     // usleep(500);
    // }
    for(int i = 0; i < _legCount; i++) {
        _legs[i].release();
    }
    _isArmed = false;
};