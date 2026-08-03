#pragma once

#include "i2cBus.hpp"

class i2cEndPoint {
  public:
    i2cEndPoint(i2cBus& bus, i2cAddr_t addr);

    bool isInited();
    bool RegRead(uint8_t RegNum, uint8_p pByte);
    bool RegWrite(uint8_t RegNum, uint8_t pByte);

    i2cAddr_t addr() const { return _addr; }

  private:
    i2cBus&   _bus;
    i2cAddr_t _addr;
};

