#include "i2c_cWrap.h"
#include <stdio.h>

#include "i2cBus_EndPiont.hpp"
// Scan all I2C addresses on the bus
int busScan() {
    i2cBus iBus(1, true);
    if (!iBus.isInited())
        return 1;

    printf("Scanning I2C bus:\n");
    for (i2cAddr_t
        addr = I2C_RESERVED_FUTURE_LOW_1;
        addr < I2C_RESERVED_10BIT_ADDR_1;
        addr = (i2cAddr_t)(addr + 1)
        ) {
        if (iBus.ProbeDevice((i2cAddr_t)addr))
            printf(" [0x%2X] \"%s\"\n",
                addr, get_i2cDevName(addr)
            );
    }
    iBus.Deinit();
    return 0;
}

// Dump register contents of a specific I2C device
int dumpAddr(uint8_t addr) {
    i2cBus iBus(1, true);
    if (!iBus.isInited())
        return 1;

    i2cEndPoint i2D(iBus, (i2cAddr_t)addr);

    printf("Device: [0x%2X] \"%s\"\n",
        addr, get_i2cDevName(addr)
    );
    for (int i = 0; i <= 0xFF; ++i) {
        uint8_t rByte;
        if (i2D.RegRead(i, &rByte))     printf("0x%02X", rByte);
        else                            printf("[NA]");
        printf("%c",
            ((i + 1) & 0x07) ?
            ' ' : '\n'
        );
    }

    iBus.Deinit();
    return 0;
}
