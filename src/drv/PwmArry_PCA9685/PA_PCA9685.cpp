#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

PCA9685::PCA9685(uint8_t bus, uint8_t address, Hertz freq) :
    _i2c_bus(bus),
    _i2c_address(address),
    PWM{
        PWMChannel(*this, 0),  PWMChannel(*this, 1),  PWMChannel(*this, 2),  PWMChannel(*this, 3),
        PWMChannel(*this, 4),  PWMChannel(*this, 5),  PWMChannel(*this, 6),  PWMChannel(*this, 7),
        PWMChannel(*this, 8),  PWMChannel(*this, 9),  PWMChannel(*this,10),  PWMChannel(*this,11),
        PWMChannel(*this,12),  PWMChannel(*this,13),  PWMChannel(*this,14),  PWMChannel(*this,15)
    },
    _channelInversion{false}
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
