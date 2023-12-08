#include "i2c.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


int busScan(){
      const char *device = "/dev/i2c-1"; // Замените на /dev/i2c-0, если необходимо
    int file;

    if ((file = open(device, O_RDWR)) < 0) {
        perror("Не удал ось открыть файл I2C");
        return 1;
    }

    printf("Сканирование шины I2C:\n");
    for (int addr = 0x03; addr <= 0x77; addr++) {
        if (ioctl(file, I2C_SLAVE, addr) < 0) {
            continue;
        }

        // Используем простую операцию чтения для проверки наличия устройства
        char buf;
        if (read(file, &buf, 1) == 1) {
            printf("Найдено устройство на адресе 0x%02x\n", addr);
        }
    }

    close(file);
    return 0;
}