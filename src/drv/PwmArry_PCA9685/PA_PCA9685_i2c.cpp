#include "PA_PCA9685.hpp"

#include <stdio.h>

#include <cerrno>    //errno
#include <cstring>   // for strerror
#include <unistd.h>  //write usleep
#include <fcntl.h>   //open
#include <sys/ioctl.h>   //ioctl
#include <linux/i2c-dev.h>   //I2C_SLAVE

void PA_PCA9685::_busInit() {
    char filename[20];
    snprintf(filename, sizeof(filename), "/dev/i2c-%d", i2c_bus);
    
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("ERROR1\n");
        fprintf(stderr,
                "[ERROR] %s: Failed to open I2C bus %d (%s). errno=%d\n",
                __func__, i2c_bus, filename, errno);
        return;
    }

    if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
        printf("ERROR2\n");
        fprintf(stderr,
                "[ERROR] %s: Failed to set I2C address 0x%02X on bus %d (%s). errno=%d\n",
                __func__, i2c_address, i2c_bus, filename, errno);
        close(fd);
        fd = -1;
        return;
    }
    printf("ERROR0 OK\n");
}

void PA_PCA9685::_busDeinit(){
    close(fd);
}

void PA_PCA9685::_writeRegister(uint8_t reg, PCA_Register value) {
    PCA_Register buf[2] = {(uint8_t)reg, value};
    if (write(fd, buf, 2) != 2) {
        printf("ERROR3\n");
        fprintf(stderr,
            "[ERROR] %s: Failed to write to register 0x%02X (value 0x%02X) "
            "at I2C addr 0x%02X on bus %d. errno=%d (%s)\n",
            __func__, reg, value, i2c_address, i2c_bus, errno, strerror(errno));
    }
}

PCA_Register PA_PCA9685::_readRegister(uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {  // Установка адреса регистра для чтения
        printf("ERROR4\n");
        fprintf(stderr,
            "[ERROR] %s: Failed to set register address 0x%02X for read "
            "at I2C addr 0x%02X on bus %d. errno=%d (%s)\n",
            __func__, reg, i2c_address, i2c_bus, errno, strerror(errno));
        return 0xFF; // error code or sentinel
    };
    PCA_Register value;
    if (read(fd, &value, 1) != 1) {
        printf("ERROR5\n");
        fprintf(stderr,
            "[ERROR] %s: Failed to read from register 0x%02X "
            "at I2C addr 0x%02X on bus %d. errno=%d (%s)\n",
            __func__, reg, i2c_address, i2c_bus, errno, strerror(errno));
        return 0xFF; // error code or sentinel
     }
    return value;
}