#include "i2c.h"
#include "types.h"
typedef enum {
    I2C_DEV_UNKNOWN = 0x00,

    /* Reserved: 0x00-0x07 - general call, CBUS, HS-mode master codes */
    I2C_RESERVED_GENERAL_CALL      = 0x00,
    I2C_RESERVED_CBUS              = 0x01,
    I2C_RESERVED_DIFFERENT_BUS_FMT = 0x02,
    I2C_RESERVED_FUTURE_LOW_1      = 0x03,
    I2C_RESERVED_HS_MASTER_CODE_1  = 0x04,
    I2C_RESERVED_HS_MASTER_CODE_2  = 0x05,
    I2C_RESERVED_HS_MASTER_CODE_3  = 0x06,
    I2C_RESERVED_HS_MASTER_CODE_4  = 0x07,

    I2C_DEV_0x0D = 0x0D,
    I2C_DEV_0x40 = 0x40,
    I2C_DEV_0x41 = 0x41,
    I2C_DEV_0x48 = 0x48,
    I2C_DEV_0x68 = 0x68,
    I2C_DEV_0x70 = 0x70,
    I2C_DEV_COUNT,

    /* Reserved: 0x78-0x7F - 10-bit addressing prefix and future use */
    I2C_RESERVED_10BIT_ADDR_1  = 0x78,
    I2C_RESERVED_10BIT_ADDR_2  = 0x79,
    I2C_RESERVED_10BIT_ADDR_3  = 0x7A,
    I2C_RESERVED_10BIT_ADDR_4  = 0x7B,
    I2C_RESERVED_FUTURE_HIGH_1 = 0x7C,
    I2C_RESERVED_FUTURE_HIGH_2 = 0x7D,
    I2C_RESERVED_FUTURE_HIGH_3 = 0x7E,
    I2C_RESERVED_FUTURE_HIGH_4 = 0x7F,

} I2cDevice_t;

static cStringRO i2cDeviceName[I2C_DEV_COUNT] = {
    [I2C_DEV_UNKNOWN] = "Unknown",
    [I2C_DEV_0x0D]    = "IMU GY273 [0x0D]",
    [I2C_DEV_0x40]    = "PWM PCA9685-A0 [0x40]",
    [I2C_DEV_0x41]    = "PWM PCA9685-A1 [0x41]",
    [I2C_DEV_0x48]    = "ADC ADS7830-A0 [0x48]",
    [I2C_DEV_0x68]    = "IMU MPU6050-A0 [0x68]",
    [I2C_DEV_0x70]    = "PWM PCA9685 All-Call [0x70]",
};

#include <stdio.h>
static inline void printI2cName(I2cDevice_t addr){
    if (i2cDeviceName[addr])    printf("device \"%s\"", i2cDeviceName[addr]);
    else                        printf("device at address 0x%02x", addr);
}

#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

static int _fd;
static inline bool setAddr(int8_t adr) { return ioctl(_fd, I2C_SLAVE, adr) < 0; }
static cStringRO _device = "/dev/i2c-1";
static inline bool openBus() {
    bool ret = (_fd = open(_device, O_RDWR)) < 0;
    if (ret)
        printf("Failed to open I2C bus\n");
    return ret;
}
static inline void closeBus() { close(_fd); }
static inline bool busRead(uint8_p pByte) {  return ( read(_fd,  pByte, 1) == 1); }
static inline bool busWrite(uint8_t pByte) { return (write(_fd, &pByte, 1) == 1); }
static inline bool setRegNum(uint8_t RegNum) {
    bool ret = busWrite(RegNum);
    if (!ret)
        printf("Failed to write register address 0x%x\n", RegNum);
    return ret;
}
static inline bool busReadRegByte(uint8_t RegNum, uint8_p pByte) {
    return 
        setRegNum(RegNum) &&
        busRead(pByte);
}
static inline bool busWriteRegByte(uint8_t RegNum, uint8_t pByte) {

    return 
        setRegNum(RegNum) &&
        busWrite(pByte);
}

// Scan all I2C addresses on the bus
int busScan() {
    if (openBus())
        return 1;

    printf("Scanning I2C bus:\n");
    for (int addr = 0x03; addr <= 0x77; addr++) {
        if (setAddr(addr))
            continue;

        // Use a simple read operation to check for device presence
        uint8_t buf;
        if (busRead(&buf)) {
            printf("Found "); printI2cName(addr); printf("\n");
        }
    }

    closeBus();
    return 0;
}

// #define I2C_BUS "/dev/i2c-1" // Use "/dev/i2c-0" or "/dev/i2c-1" depending on your system
// #define I2C_ADDR 0x68 // Replace with your device's I2C address
#define REG_ADDR 0x00 // Starting register address for reading
#define NUM_REGS 0x45 // Number of registers to read

// Dump register contents of a specific I2C device
int dumpAddr(uint8_t addr) {
    if (openBus())  // Open the I2C bus
        return 1;

    if (setAddr(addr)) {        // Set the target device address
        printf("Failed to connect to I2C device\n");
        closeBus();
        return 1;
    }

    // Read registers one by one
    uint8_t buffer[128] = {0};
    for (int i = 0; i < NUM_REGS; ++i)
        if (!busReadRegByte(i, &buffer[i]))
            printf("Failed to read from I2C device\n");

    // Example: write data 0xFF 0xAA to register 0x06
    uint8_t reg[3];
    reg[0] = 0x06;
    reg[1] = 0xFF;
    reg[2] = 0xAA;

    if (!busWriteRegByte(reg[0], reg[1]))
        printf("Failed to write to register 0x%x\n", reg[0]);

    // Print collected data
    printf("Data from I2C "); printI2cName(addr); printf(":\n");

    for (int i = 0; i < sizeof(buffer); i++) {
        printf("0x%02x ", buffer[i]);
        if (((i + 1) % 16) == 0)
            printf("\n");
    }

    closeBus();
    return 0;
}

