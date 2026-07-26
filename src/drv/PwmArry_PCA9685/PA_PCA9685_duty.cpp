#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include <iomanip>   //std::setfill
#include <bitset>    //std::bitset
#include "terminal_tools.h"

void PCA9685::setDutyCycle(
    uint8_t channel,
    DutyCycle_t dutyCycle,
    DutyCycle_t phaseShift
) {
    // LOG("setDC %f\n", dutyCycle);

    clampDuty(&dutyCycle);
    clampDuty(&phaseShift);

    uint16_t on = MAX_VAL * phaseShift;
    uint16_t off = on + (MAX_VAL * dutyCycle);
    off %= MAX_VAL;

    if (_channelInversion[channel]) {
        std::swap(on, off);
    }

    if (channel < LED_NUM) {
        _writeRegister(LED0_ON_L  + (4 * channel), on & 0xFF);
        _writeRegister(LED0_ON_H  + (4 * channel), on >> 8);
        _writeRegister(LED0_OFF_L + (4 * channel), off & 0xFF);
        _writeRegister(LED0_OFF_H + (4 * channel), off >> 8);
    }
}

DutyCycle_t PCA9685::getDutyCycle(uint8_t channel) {
    if (!(channel < LED_NUM)) {
        printf("Error: Channel number out of range (0-15).\n");
        return -1;
    }

    int onValue     = _readRegister(LED0_ON_L  + (4 * channel)) |
                     (_readRegister(LED0_ON_H  + (4 * channel)) << 8);
    int offValue    = _readRegister(LED0_OFF_L + (4 * channel)) |
                     (_readRegister(LED0_OFF_H + (4 * channel)) << 8);

    return
        (offValue - onValue + (
            (offValue >= onValue) ?   // TODO: make it crear
             0.f : 1.f
        ) * 1.f) /
        MAX_VAL;
}