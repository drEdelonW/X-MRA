

#include "types.h"
typedef enum {
    I2C_DEV_UNKNOWN = 0x00,

    I2C_DEV_0x0D = 0x0D,
    I2C_DEV_0x40 = 0x40,
    I2C_DEV_0x41 = 0x41,
    I2C_DEV_0x48 = 0x48,
    I2C_DEV_0x68 = 0x68,
    I2C_DEV_0x70 = 0x70,
    I2C_DEV_COUNT,
} I2cKnownDevice_t;

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
void printI2cName(int8_t addr){
    if (i2cDeviceName[addr])    printf("device \"%s\"", i2cDeviceName[addr]);
    else                        printf("device at address 0x%02x", addr);
}