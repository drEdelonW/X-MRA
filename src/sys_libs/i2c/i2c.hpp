#pragma once
#include "types.h"

typedef int8_t  i2cAddr_t;  // negative not allowed - upper bit is data direction status

class i2cBus {
  public:
    i2cBus(uint8_t bus = 0, bool initOnConstruct = false);
    bool Init();
    void Deinit();
    bool isInited() { return _isInited; }

  private:
    bool      _isInited;
    uint8_t   _bus;
    i2cAddr_t _lastAddr;
    int       _fd;
    char      _fileName[20];

    bool _setAddres(i2cAddr_t addr);
};
