#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"
#include "terminal_tools.h"

#include <cmath>     //round floor

MicroSeconds PCA9685::_getDutyCyclePeriodUs(Hertz frequencyHz) {
    if (frequencyHz <= Hz(0.f))
        return us(0);
    return periodUsOf(frequencyHz);
}

void PCA9685::setDuration(
    PwmChannel channel,
    MicroSeconds duration,
    MicroSeconds phaseShift
) {
    // LOG("setDur frq[%f]Hz dur[%ld]us per[%ld]us\n", _freq, duration, _periodUs);
    setDutyCycle(channel, (1.f * duration) / _periodUs);
    // setDutyCycle(channel, 0.1f );
    // setDutyCycle(channel, 0.05f );
}

MicroSeconds PCA9685::getDuration(PwmChannel channel) { return us(0); }

