#include "PA_PCA9685.hpp"

#include <stdio.h>

#include <unistd.h>  //write usleep
#include <fcntl.h>   //open
#include <sys/ioctl.h>   //ioctl
#include <linux/i2c-dev.h>   //I2C_SLAVE

void PA_PCA9685::_busInit(){
    // Открытие шины I2C
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", i2c_bus);
    fd = open(filename, O_RDWR);

    if (fd < 0) {
        // Обработка ошибки открытия файла
    }

    // Установка адреса устройства
    if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
        // Обработка ошибки соединения
    }
}

void PA_PCA9685::_busDeinit(){
    close(fd);
}

void PA_PCA9685::_writeRegister(uint8_t reg, PCA_Register value) {
    PCA_Register buf[2] = {(uint8_t)reg, value};
    if (write(fd, buf, 2) != 2) {
        // Обработка ошибки записи
    }
}

PCA_Register PA_PCA9685::_readRegister(uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {  // Установка адреса регистра для чтения
        // Обработка ошибки чтения
    };
    PCA_Register value;
    if (read(fd, &value, 1) != 1) {
        // Обработка ошибки чтения
    }
    return value;
}