#include "pca9685_obj.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>

class PCA9685 {
public:
    PCA9685(int bus, int address) : i2c_bus(bus), i2c_address(address) {
        // Открытие шины I2C
        char filename[20];
        snprintf(filename, 19, "/dev/i2c-%d", bus);
        file = open(filename, O_RDWR);

        if (file < 0) {
            // Обработка ошибки открытия файла
        }

        // Установка адреса устройства
        if (ioctl(file, I2C_SLAVE, address) < 0) {
            // Обработка ошибки соединения
        }
    }

    ~PCA9685() {
        close(file);
    }

    // Установка частоты ШИМ
    void setPWMFreq(int freq) {
        float prescale_val = 25000000.0; // 25MHz
        prescale_val /= 4096.0;          // 12-bit
        prescale_val /= float(freq);
        prescale_val -= 1.0;

        unsigned char prescale = floor(prescale_val + 0.5);

        unsigned char old_mode = readRegister(0x00); // Чтение MODE1 регистра
        unsigned char new_mode = (old_mode & 0x7F) | 0x10; // Sleep
        writeRegister(0x00, new_mode); // Переход в режим сна
        writeRegister(0xFE, prescale); // Установка регистра предделителя
        writeRegister(0x00, old_mode);
        usleep(5000); // Задержка для установки частоты
        writeRegister(0x00, old_mode | 0x80); // Включение
    }

    // Установка скважности ШИМ
    void setPWMDutyCycle(int channel, int on, int off) {
        writeRegister(0x06 + 4 * channel, on & 0xFF);
        writeRegister(0x07 + 4 * channel, on >> 8);
        writeRegister(0x08 + 4 * channel, off & 0xFF);
        writeRegister(0x09 + 4 * channel, off >> 8);
    }

private:
    int file;
    int i2c_bus;
    int i2c_address;

    // Функция для записи в регистр
    void writeRegister(unsigned char reg, unsigned char value) {
        unsigned char buf[2] = {reg, value};
        if (write(file, buf, 2) != 2) {
            // Обработка ошибки записи
        }
    }

    // Функция для чтения из регистра
    unsigned char readRegister(unsigned char reg) {
        write(file, &reg, 1); // Установка адреса регистра для чтения
        unsigned char value;
        if (read(file, &value, 1) != 1) {
            // Обработка ошибки чтения
        }
        return value;
    }
};

// int main() {
//     // Пример использования
//     PCA9685 pca9685(1, 0x40); // Указываем номер шины и адрес устройства
//     pca9685.setPWMFreq(1000); // Установка частоты ШИМ
//     pca9685.setPWMDutyCycle(0, 0, 2048); // Установка скважности для канала 0

//     return 0;
// }
