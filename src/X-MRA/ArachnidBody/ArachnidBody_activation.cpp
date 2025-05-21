#include "ArachnidBody.hpp"
#include <unistd.h>  //usleep
#include "terminal_tools.h"

bool ArachnidBody::isArmed() {
    return _isArmed;
}

bool ArachnidBody::ARM() {
    if (_isArmed) {
        LOG("ARMED\n");
        return true;
    }
    LOG("ARMING... ");

    for(int i = 0; i < _legCount; i++) {
        _legs[i].engage();
        _legs[i].checkTipPosBodySpace( _legExtras[i].defaultPose );
    }

    LOG("DONE\n");
    return _isArmed = true;
};


void ArachnidBody::DISARM() {
    if (!_isArmed) {
        LOG("DISARMED\n");
        return;
    }
    LOG("DISARMING... ");

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
    for(int i = 0; i < _legCount; i++) {
        _legs[i].release();
    }
    _isArmed = false;
    LOG("DONE\n");
};