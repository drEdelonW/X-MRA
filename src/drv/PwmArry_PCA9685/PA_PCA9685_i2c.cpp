#include "PA_PCA9685.hpp"


PCA_reg_t PCA9685::_readRegister(uint8_t reg) {
    PCA_reg_t value;
    _iEP.RegRead(reg, &value);
    return value;
}