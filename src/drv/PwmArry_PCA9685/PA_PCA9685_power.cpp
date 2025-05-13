#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include <unistd.h>  //usleep

void PCA9685::wakeUp() {
    PCA_Register mode1 = _readRegister(MODE1);
    mode1 &= ~(1 << 4);    // Сброс бита сна (бит 4)
    _writeRegister(MODE1, mode1);
    usleep(500);    // Ждем 500 микросекунд, чтобы убедиться в стабилизации осциллятора
}

void PCA9685::sleepMode() {
    PCA_Register mode1 = _readRegister(MODE1);
    mode1 |= (1 << 4);  // Установка бита сна (бит 4)
    _writeRegister(MODE1, mode1);
}