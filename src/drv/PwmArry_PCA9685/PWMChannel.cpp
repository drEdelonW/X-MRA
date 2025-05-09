#include "PWMChannel.hpp"
#include <stdio.h>
PWMChannel::PWMChannel(ProtoPWMArray& ctrl, uint8_t ch)
    : controller(ctrl), channel(ch) {}


void PWMChannel::setPulseDurationUS(MicroSeconds duration_us) {
    setDutyCycle(400);
};
MicroSeconds PWMChannel::getPulseDurationUS() {
    return 0;
};

void      PWMChannel::setDutyCycle(DutyCycle duty) {
    controller.setDutyCycle(channel, duty);
};
DutyCycle PWMChannel::getDutyCycle() {
    return controller.getDutyCycle(channel);
};

MicroSeconds PWMChannel::getPeriodUS() {
    return 0;
};

void PWMChannel::disable() {};
void PWMChannel::enable() {};
bool PWMChannel::isEnabled() { return false; };