#include "types.h"
typedef enum {
    I2C_DEV_UNKNOWN = 0x00,

    I2C_DEV_0x0D = 0x0D,
    I2C_DEV_0x40 = 0x40,
    I2C_DEV_0x41 = 0x41,
    I2C_DEV_0x48 = 0x48,
    I2C_DEV_0x4B = 0x4B,
    I2C_DEV_0x53 = 0x53,
    I2C_DEV_0x68 = 0x68,
    I2C_DEV_0x70 = 0x70,
    I2C_DEV_COUNT,
} I2cKnownDevice_t;

static cStrRO _i2cDeviceName[I2C_DEV_COUNT] = {
    [I2C_DEV_UNKNOWN] = "Unknown",
    [I2C_DEV_0x0D]    = "IMU GY273",
    [I2C_DEV_0x40]    = "PWM PCA9685-A0",
    [I2C_DEV_0x41]    = "PWM PCA9685-A1",
    [I2C_DEV_0x48]    = "ADC ADS7830-A0",
    [I2C_DEV_0x4B]    = "IMU BNO080",
    [I2C_DEV_0x53]    = "IMU ADXL345",
    [I2C_DEV_0x68]    = "IMU MPU6050-A0",
    [I2C_DEV_0x70]    = "PWM PCA9685 All-Call",
};

cStrRO get_i2cDevName(int8_t adr) {
    if ((adr < I2C_DEV_COUNT) &&
        (adr > I2C_DEV_UNKNOWN) &&
        (_i2cDeviceName[adr] != NULL)
    )   return _i2cDeviceName[adr];
    else
        return _i2cDeviceName[I2C_DEV_UNKNOWN];
}
