#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

PCA9685::PCA9685(uint8_t bus, PCA_Addr address, Hertz freq) :
    _initted(false),
    _i2c_bus(bus),
    _i2c_address(address),
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
    _busInit();
    if (_initted) {
        setFreq_Hz(freq);
        _freq = _readFreq_Hz();
        _periodUs = _getDutyCyclePeriodUs(_freq);
        wakeUp();
        // printStatus();
    } else {
        _busDeinit();
    }
}

PCA9685::~PCA9685() {
    if (_initted) {
        sleepMode();
    }
    _busDeinit();
}


void PCA9685::setInversion(uint8_t channel, bool inverted) {
    if (channel < LED_NUM) {
        _channelInversion[channel] = inverted;
    }
}

bool PCA9685::getInversion(uint8_t channel) {
    if (channel < LED_NUM) {
        return _channelInversion[channel];
    }
    return false;
}
