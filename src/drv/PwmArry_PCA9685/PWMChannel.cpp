#include "PWMChannel.hpp"
#include <stdio.h>
// #include "terminal_tools.h"

PWMChannel::PWMChannel(ProtoPWMArray& ctrl, uint8_t ch)
    : controller(ctrl), channel(ch) {}


void PWMChannel::setPulseDurationUS(MicroSeconds duration_us) {
    setDuration(duration_us);
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

void      PWMChannel::setDuration(MicroSeconds duration) {
    // LOG("setDur1 %d\n",duration);
    controller.setDuration(channel, duration);
};
MicroSeconds PWMChannel::getDuration() {
    return controller.getDuration(channel);
};

MicroSeconds PWMChannel::getPeriodUS() {
    return 0;
};

void PWMChannel::disable() {};
void PWMChannel::enable() {};
bool PWMChannel::isEnabled() { return false; };