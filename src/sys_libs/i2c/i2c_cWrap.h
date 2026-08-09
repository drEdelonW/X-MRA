#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

    void busScan();
    int dumpAddr(uint8_t addr);
    cStrRO get_i2cDevName(int8_t adr);

#ifdef __cplusplus
}
#endif