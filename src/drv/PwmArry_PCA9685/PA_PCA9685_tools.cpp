#include "PA_PCA9685.hpp"

#include "common_tools.h"

void PA_PCA9685::printStatus() {
    PCA_Register mode1      = _readRegister(MODE1);
    PCA_Register mode2      = _readRegister(MODE2);
    PCA_Register prescale   = _readRegister(PRE_SCALE);
    
    float oscFreq = 25000000.0; // Осцилляторная частота в Гц
    float pwmFreq = oscFreq / (4096.0 * (prescale + 1)); // Расчет частоты ШИМ

    printf("Extended state of PA_PCA9685[%X]:\n",i2c_address);
    printf("MODE1: 0b" BYTE_TO_BINARY_PATTERN " (sleep mode: %s)\n",
           BYTE_TO_BINARY(mode1),
           (mode1 & 0x10) ? "ON" : "OFF");
    
    printf("MODE2: 0b" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(mode2));
    printf("PRE_SCALE: %u (Approx. PWM frequency: %.2f Hz)\n", prescale, pwmFreq);
    
    for (int channel = 0; channel < 16; ++channel) {
        int onValue  = _readRegister(LED0_ON_L + 4 * channel) | (_readRegister(LED0_ON_H + 4 * channel) << 8);
        int offValue = _readRegister(LED0_OFF_L + 4 * channel) | (_readRegister(LED0_OFF_H + 4 * channel) << 8);
    
        printf("Channel %02d: ON = %04d, OFF = %04d\n", channel, onValue, offValue);
    }
}