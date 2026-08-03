#include "PA_PCA9685.hpp"

void PCA9685::_writeRegister(uint8_t reg, PCA_reg_t value) {
    _iEP.RegWrite(reg, value);
}

PCA_reg_t PCA9685::_readRegister(uint8_t reg) {
    PCA_reg_t value;
    _iEP.RegRead(reg, &value);
    return value;
}