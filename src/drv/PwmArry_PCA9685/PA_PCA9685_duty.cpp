#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include "terminal_tools.h"

void PCA9685::setDutyCycle(
    PwmChannel channel,
    DutyCycle_t dutyCycle,
    DutyCycle_t phaseShift
) {
    // LOG("setDC %f\n", dutyCycle);

    clampDuty(&dutyCycle);
    clampDuty(&phaseShift);

    uint16_t on  = (MAX_VAL * phaseShift);
    uint16_t off = (MAX_VAL * dutyCycle) + on;
    off %= MAX_VAL;

    if (_channelInversion[channel]) {
        uint16_t t = on;
        on = off;
        off = t;
    }

    if (channel < PwmChNum) {
        int rOffs = channel * 4;
        _iEP.RegWrite(LED0_ON_L + rOffs, on & 0xFF);
        _iEP.RegWrite(LED0_ON_H + rOffs, on >> 8);

        _iEP.RegWrite(LED0_OFF_L + rOffs, off & 0xFF);
        _iEP.RegWrite(LED0_OFF_H + rOffs, off >> 8);
    }
}

DutyCycle_t PCA9685::getDutyCycle(PwmChannel channel) {
    if (!(channel < PwmChNum)) {
        printf("Error: Channel number out of range (0-15).\n");
        return -1;
    }
    int rOffs = channel * 4;
    uint16_t onValue =
        (_readRegister(LED0_ON_L + rOffs)) |
        (_readRegister(LED0_ON_H + rOffs) << 8);
    uint16_t offValue =
        (_readRegister(LED0_OFF_L + rOffs)) |
        (_readRegister(LED0_OFF_H + rOffs) << 8);

    return
        (offValue - onValue +
            ((offValue >= onValue) ?   // TODO: make it clear
                0.f : 1.f) *
            1.f) /
        MAX_VAL;
}