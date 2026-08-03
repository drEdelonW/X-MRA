#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"
#include "terminal_tools.h"

PCA9685::PCA9685(i2cBus& bus, PCA_Addr address, Hertz freq) :
#if 0
    _initted(false),
    _i2c_bus(bus),
    _i2c_address(address),
#else
    // _bus(bus),
    _iEP(bus, (i2cAddr_t)address),
#endif
    PWM{
        PWMChannel(*this, PwmCh0),  PWMChannel(*this, PwmCh1),
        PWMChannel(*this, PwmCh2),  PWMChannel(*this, PwmCh3),
        PWMChannel(*this, PwmCh4),  PWMChannel(*this, PwmCh5),
        PWMChannel(*this, PwmCh6),  PWMChannel(*this, PwmCh7),
        PWMChannel(*this, PwmCh8),  PWMChannel(*this, PwmCh9),
        PWMChannel(*this, PwmChA),  PWMChannel(*this, PwmChB),
        PWMChannel(*this, PwmChC),  PWMChannel(*this, PwmChD),
        PWMChannel(*this, PwmChE),  PWMChannel(*this, PwmChF)
    },
    _channelInversion{false},
    _freq(Hz(0.f)),
    _periodUs(us(0))
{
    if (_iEP.isInited()) {
        setFreq_Hz(freq);
        _freq = _readFreq_Hz();
        _periodUs = _getDutyCyclePeriodUs(_freq);
        wakeUp();
        // printStatus();
    } else {
        LOG("Bus NOT INITED\n");
    }
}

PCA9685::~PCA9685() {
    if (_iEP.isInited())
        sleepMode();
}


void PCA9685::setInversion(PwmChannel channel, bool inverted) {
    if (channel < PwmChNum) {
        _channelInversion[channel] = inverted;
    }
}

bool PCA9685::getInversion(PwmChannel channel) {
    if (channel < PwmChNum) {
        return _channelInversion[channel];
    }
    return false;
}
