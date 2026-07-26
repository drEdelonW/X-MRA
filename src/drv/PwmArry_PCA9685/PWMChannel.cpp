#include "PWMChannel.hpp"
#include <stdio.h>

PWMChannel::PWMChannel(ProtoPWMArray& ctrl, uint8_t ch)
: controller(ctrl), channel(ch) {}


void        PWMChannel::setDutyCycle(DutyCycle_t duty) {
    controller.setDutyCycle(channel, duty);
};
DutyCycle_t PWMChannel::getDutyCycle() {
    return controller.getDutyCycle(channel);
};

// #include "terminal_tools.h"
void         PWMChannel::setDuration(MicroSeconds duration) {
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