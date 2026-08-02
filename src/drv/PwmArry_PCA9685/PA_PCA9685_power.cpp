#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include <unistd.h>  //usleep

void PCA9685::wakeUp() {
    _writeRegister(MODE1, _readRegister(MODE1) & ~SLEEP_BIT);       usleep(500);
}

void PCA9685::sleepMode() {
    _writeRegister(MODE1, _readRegister(MODE1) | SLEEP_BIT);
}