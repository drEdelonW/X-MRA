#pragma once
#include "i2cBus_EndPiont.hpp"

typedef enum : uint8_t {
    ADCaddr_0 = 0x48
} ADS7830_Addr;

class ADS7830 {
  public:
    ADS7830(i2cBus& bus, ADS7830_Addr address = ADCaddr_0) :
        _iEP(bus, (i2cAddr_t)address) {}

    uint8_t readChannel(uint8_t channel); // channel 0-7, raw 8-bit code (0-255)
    void    readAll(uint8_t out[8]);

  private:
    i2cEndPoint _iEP;
};
