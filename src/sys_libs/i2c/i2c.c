#include "i2c.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// Scan all I2C addresses on the bus
int busScan() {
    const char *device = "/dev/i2c-1"; // Change to "/dev/i2c-0" if needed
    int file;

    if ((file = open(device, O_RDWR)) < 0) {
        printf("Failed to open I2C bus\n");
        return 1;
    }

    printf("Scanning I2C bus:\n");
    for (int addr = 0x03; addr <= 0x77; addr++) {
        if (ioctl(file, I2C_SLAVE, addr) < 0) {
            continue;
        }

        // Use a simple read operation to check for device presence
        char buf;
        if (read(file, &buf, 1) == 1) {
            printf("Found device at address 0x%02x\n", addr);
        }
    }

    close(file);
    return 0;
}

// #define I2C_BUS "/dev/i2c-1" // Use "/dev/i2c-0" or "/dev/i2c-1" depending on your system
// #define I2C_ADDR 0x68 // Replace with your device's I2C address
#define REG_ADDR 0x00 // Starting register address for reading
#define NUM_REGS 0x45 // Number of registers to read

// Dump register contents of a specific I2C device
int dumpAddr(char addr) {
    int file;
    const char *device = "/dev/i2c-1";
    unsigned char buffer[128] = {0};
    unsigned char reg[3];
    reg[0] = REG_ADDR;

    // Open the I2C bus
    if ((file = open(device, O_RDWR)) < 0) {
        printf("Failed to open I2C bus\n");
        return 1;
    }

    // Set the target device address
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        printf("Failed to connect to I2C device\n");
        close(file);
        return 1;
    }

    // Read registers one by one
    for (int i = 0; i < NUM_REGS; ++i) {
        reg[0] = REG_ADDR + i;

        // Write the register address
        if (write(file, reg, 1) != 1) {
            printf("Failed to write register address 0x%x\n", reg[0]);
            continue;
        }

        // Read data from the register
        if (read(file, &buffer[i], 1) != 1) {
            printf("Failed to read from I2C device\n");
            continue;
        }
    }

    // Example: write data 0xFF 0xAA to register 0x06
    reg[0] = 6;
    reg[1] = 0xFF;
    reg[2] = 0xAA;
    if (write(file, &reg, 2) != 2) {
        printf("Failed to write to register 0x%x\n", reg[0]);
    }

    // Print collected data
    printf("Data from I2C device at address 0x%02x:\n", addr);
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("0x%02x ", buffer[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    close(file);
    return 0;
}

void busScan_() {
    busScan();
}
