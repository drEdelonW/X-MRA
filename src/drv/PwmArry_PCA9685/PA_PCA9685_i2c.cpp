#include "PA_PCA9685.hpp"

#include <stdio.h>

#include <cerrno>           //errno
#include <cstring>          //strerror

#include <unistd.h>         //write usleep
#include <fcntl.h>          //open
#include <sys/ioctl.h>      //ioctl
#include <linux/i2c-dev.h>  //I2C_SLAVE

#define I2C_BUS_ID_PATTERN "at I2C addr 0x%02X on bus %d. errno=%d (%s)\n"
#define I2C_BUS_ID_ARGS __func__, reg, _i2c_address, _i2c_bus, errno, strerror(errno)

void PCA9685::_busInit() {
    char filename[20];
    snprintf(filename, sizeof(filename), "/dev/i2c-%d", _i2c_bus);

    _fd = open(filename, O_RDWR);
    if (_fd < 0) {
        fprintf(stderr,
                "[ERROR] %s: Failed to open I2C bus %d (%s). errno=%d\n",
                __func__, _i2c_bus, filename, errno);
        return;
    }

    if (ioctl(_fd, I2C_SLAVE, _i2c_address) < 0) {
        fprintf(stderr,
            "[ERROR] %s: Failed to set I2C address 0x%02X on bus %d (%s). errno=%d\n",
            __func__, _i2c_address, _i2c_bus, filename, errno
        );
        close(_fd); _fd = -1;
        return;
    }

    // Try to read MODE1 register (0x00) to check presence
    PCA_Register reg = 0x00;
    uint8_t value = 0;
    if ((write(_fd, &reg, 1) != 1 ) ||
        (read(_fd, &value, 1) != 1)
    ) {
        fprintf(stderr,
            "[ERROR] %s: Device not responding at I2C address 0x%02X on bus %d. errno=%d (%s)\n",
            __func__, _i2c_address, _i2c_bus, errno, strerror(errno)
        );
        close(_fd); _fd = -1;
        return;
    }

    // Success
    // fprintf(stderr,
    //     "[INFO] %s: PCA9685 found at I2C address 0x%02X on bus %d.\n",
    //     __func__, _i2c_address, _i2c_bus
    // );
    _initted = true;
}

void PCA9685::_busDeinit() {
    if (_fd != -1){
        close(_fd);
    }
    _initted = false;
}

void PCA9685::_writeRegister(PCA_Register reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if ((!_initted) ||
        (write(_fd, buf, 2) != 2)
    ) {
        fprintf(stderr,
            "[ERROR] %s: Failed to write to register 0x%02X (value 0x%02X) "
            I2C_BUS_ID_PATTERN, __func__, reg, value, I2C_BUS_ID_ARGS);
    }
}

uint8_t PCA9685::_readRegister(PCA_Register reg) {
    if ((!_initted) ||
        (write(_fd, &reg, 1) != 1)
    ) {
        fprintf(stderr,
            "[ERROR] %s: Failed to set register address 0x%02X for read "
            I2C_BUS_ID_PATTERN, __func__, reg, I2C_BUS_ID_ARGS);
        return 0xFF; // error code or sentinel
    };
    uint8_t value;
    if (read(_fd, &value, 1) != 1) {
        fprintf(stderr,
            "[ERROR] %s: Failed to read from register 0x%02X "
            I2C_BUS_ID_PATTERN, __func__, reg, I2C_BUS_ID_ARGS);
        return 0xFF; // error code or sentinel
     }
    return value;
}