#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include "common_tools.h"

void PCA9685::printStatus() {
    PCA_reg_t mode1     = _readRegister(MODE1);
    PCA_reg_t mode2     = _readRegister(MODE2);
    PCA_reg_t prescale  = _readRegister(PRE_SCALE);

    Hertz pwmFreq = Hz(OSC_FREQ_HZ /
        (MAX_VAL * (prescale + 1)));

    printf("Extended state of PCA9685[%X]:\n", _i2c_address);
    printf(
        "MODE1: " BYTE_TO_BINARY_PATTERN " (sleep mode: %s)\n",
        BYTE_TO_BINARY(mode1),
        ((mode1 & SLEEP_BIT) ?
            "ON" : "OFF")
    );

    printf("MODE2: " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(mode2));
    printf(
        "PRE_SCALE: %u "
        "(Approx. PWM frequency: %.2f Hz)\n",
        prescale,
        pwmFreq.hzValue
    );

    for (int channel = 0; channel < PwmChNum; ++channel) {
        int rOffs = channel * 4;
        uint16_t onValue =
            (_readRegister(LED0_ON_L + rOffs)) |
            (_readRegister(LED0_ON_H + rOffs) << 8);
        uint16_t offValue =
            (_readRegister(LED0_OFF_L + rOffs)) |
            (_readRegister(LED0_OFF_H + rOffs) << 8);

        printf(
            "Channel %02d: "
            "ON = %04d, "
            "OFF = %04d\n",
            channel,
            onValue,
            offValue
        );
    }
}