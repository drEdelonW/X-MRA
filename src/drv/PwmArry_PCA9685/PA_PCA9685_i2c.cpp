#include "PA_PCA9685.hpp"

#include <stdio.h>

#include <cerrno>           //errno
#include <cstring>          //strerror

#include <unistd.h>         //write usleep
#include <fcntl.h>          //open
#include <sys/ioctl.h>      //ioctl
#include <linux/i2c-dev.h>  //I2C_SLAVE

#define I2C_BUS_ID_PATTERN "at I2C addr 0x%02X on bus %d. errno=%d (%s)\n"
#define I2C_BUS_ID_ARGS __func__, reg, i2c_address, i2c_bus, errno, strerror(errno)

void PCA9685::_busInit() {
    char filename[20];
    snprintf(filename, sizeof(filename), "/dev/i2c-%d", i2c_bus);

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        fprintf(stderr,
                "[ERROR] %s: Failed to open I2C bus %d (%s). errno=%d\n",
                __func__, i2c_bus, filename, errno);
        return;
    }

    if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
        fprintf(stderr,
                "[ERROR] %s: Failed to set I2C address 0x%02X on bus %d (%s). errno=%d\n",
                __func__, i2c_address, i2c_bus, filename, errno);
        close(fd);
        fd = -1;
        return;
    }
}

void PCA9685::_busDeinit(){
    close(fd);
}

void PCA9685::_writeRegister(PCA_Register reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if (write(fd, buf, 2) != 2) {
        fprintf(stderr,
            "[ERROR] %s: Failed to write to register 0x%02X (value 0x%02X) "
            I2C_BUS_ID_PATTERN, __func__, reg, value, I2C_BUS_ID_ARGS);
    }
}

uint8_t PCA9685::_readRegister(PCA_Register reg) {
    if (write(fd, &reg, 1) != 1) {  // Установка адреса регистра для чтения
        fprintf(stderr,
            "[ERROR] %s: Failed to set register address 0x%02X for read "
            I2C_BUS_ID_PATTERN, __func__, reg, I2C_BUS_ID_ARGS);
        return 0xFF; // error code or sentinel
    };
    uint8_t value;
    if (read(fd, &value, 1) != 1) {
        fprintf(stderr,
            "[ERROR] %s: Failed to read from register 0x%02X "
            I2C_BUS_ID_PATTERN, __func__, reg, I2C_BUS_ID_ARGS);
        return 0xFF; // error code or sentinel
     }
    return value;
}