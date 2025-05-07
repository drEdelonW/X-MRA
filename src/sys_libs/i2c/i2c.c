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
        printf("Не удал ось открыть файл I2C");
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

// #define I2C_BUS "/dev/i2c-1" // Используйте "/dev/i2c-0" или "/dev/i2c-1", в зависимости от вашей системы
// #define I2C_ADDR 0x68 // Замените на адрес вашего I2C устройства
#define REG_ADDR 0x00 // Начальный адрес регистра для чтения
#define NUM_REGS 0x45 // Количество регистров для чтения

int dumpAddr(char addr){
     int file;
     const char *device = "/dev/i2c-1";
    unsigned char buffer[128] = {0};
    unsigned char reg[3];
    reg[0]  = REG_ADDR;

    // Открываем I2C шину
    if ((file = open(device, O_RDWR)) < 0) {
        printf("Ошибка открытия шины I2C\n");
        return 1;
    }

    // Указываем адрес устройства
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        printf("Ошибка при подключении к устройству I2C\n");
        close(file);
        return 1;
    }

    // Побайтовое чтение регистров
    for (int i = 0; i < NUM_REGS; ++i) {
        reg[0] = REG_ADDR + i;

        // Записываем адрес регистра
        if (write(file, reg, 1) != 1) {
            printf("Ошибка при установке адреса регистра для чтения %x\n",reg[0]);
            continue;
        }

        // Чтение данных из регистра
        if (read(file, &buffer[i], 1) != 1) {
            printf("Ошибка чтения из устройства I2C\n");
            continue;
        }

        // Печатаем данные
        // printf("Регистр 0x%02x: 0x%02x\n", reg, buffer);
    }

    reg[0] = 6;
    reg[1] = 0xFF;
    reg[2] = 0xAA;
    if (write(file, &reg, 2) != 2) {
        printf("Ошибка при установке адреса регистра для чтения %x\n",reg[0]);
    }
    // Печатаем данные
    printf("Данные с устройства I2C по адресу 0x%02x:\n", addr);
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("0x%02x ", buffer[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    close(file);
    return 0;
}