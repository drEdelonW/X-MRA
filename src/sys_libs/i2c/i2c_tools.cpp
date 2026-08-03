#include "i2c.hpp"
#include "i2c_cWrap.h"
#include <stdio.h>

// Scan all I2C addresses on the bus
int busScan() {
    i2cBus iBus(1, true);
    if (!iBus.isInited())
        return 1;

    printf("Scanning I2C bus:\n");
    for (int addr = 0x03; addr <= 0x77; addr++)
        if (iBus.ProbeDevice((i2cAddr_t)addr)) {
            printf("Found "); printI2cName(addr); printf("\n");
        }

    iBus.Deinit();
    return 0;
}


// Dump register contents of a specific I2C device
int dumpAddr(uint8_t addr) {
    i2cBus iBus(1, true);
    if (!iBus.isInited())         // Open the I2C bus
        return 1;

    if (!iBus.setAddr((i2cAddr_t)addr)) {    // Set the target device address
        printf("Failed to connect to I2C device\n");
        iBus.Deinit();
        return 1;
    }

    // Read registers one by one
    printf("\nData from I2C "); printI2cName(addr); printf(":\n");
    for (int i = 0; i <= 0xFF; ++i) {
        uint8_t rByte;
        if (iBus.RegRead(i, &rByte))    printf("0x%02X", rByte);
        else                            printf("[NA]");
        printf("%c", ((i + 1) % 8) ? ' ' : '\n');
    }

    iBus.Deinit();
    return 0;
}
