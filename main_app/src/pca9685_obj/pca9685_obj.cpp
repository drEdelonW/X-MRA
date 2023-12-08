#include "pca9685_obj.h"

#include <iostream>
#include <iomanip>
#include <bitset>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>

// #define PRE_SCALE   (0xFE)
// #define MODE1       (0x00)
// #define LED0_ON_L   (0x06)
// #define LED0_ON_H   (0x07)
// #define LED0_OFF_L  (0x08)
// #define LED0_OFF_H  (0x09)


PCA9685::PCA9685(uint8_t bus, uint8_t address) : i2c_bus(bus), i2c_address(address) {
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

PCA9685::~PCA9685() {
    close(file);
}
// Метод для вывода состояния регистров
void PCA9685::printStatus() {
    // Чтение регистра MODE1
    int mode1 = readRegister(MODE1);
    // Чтение регистра MODE2
    int mode2 = readRegister(MODE2);
    // Чтение регистра предделителя
    int prescale = readRegister(PRE_SCALE);
    
    float oscFreq = 25000000.0; // Осцилляторная частота в Гц
    float pwmFreq = oscFreq / (4096.0 * (prescale + 1)); // Расчет частоты ШИМ

    std::cout << "Расширенное состояние PCA9685:" << std::endl;
    std::cout << "MODE1: " << std::bitset<8>(mode1) << " (режим сна: " << ((mode1 & 0x10) ? "включен" : "выключен") << ")" << std::endl;
    std::cout << "MODE2: " << std::bitset<8>(mode2) << std::endl;
    std::cout << "PRE_SCALE: " << prescale << " (Приблизительная частота ШИМ: " << pwmFreq << " Гц)" << std::endl;


    for (int channel = 0; channel < 16; ++channel) {
        int onValue = readRegister(LED0_ON_L + 4 * channel) | (readRegister(LED0_ON_H + 4 * channel) << 8);
        int offValue = readRegister(LED0_OFF_L + 4 * channel) | (readRegister(LED0_OFF_H + 4 * channel) << 8);

        std::cout << "Канал " << std::setw(2) << std::setfill('0') << channel 
                    << ": ON = " << std::setw(4) << std::setfill('0') << onValue 
                    << ", OFF = " << std::setw(4) << std::setfill('0') << offValue << std::endl;
    }
}

// Метод для вывода микросхемы из режима сна
void PCA9685::wakeUp() {
    int mode1 = readRegister(MODE1);
    // Сброс бита сна (бит 4)
    mode1 &= ~(1 << 4);
    writeRegister(MODE1, mode1);

    // Ждем 500 микросекунд, чтобы убедиться в стабилизации осциллятора
    usleep(500);
}

// Метод для перевода микросхемы в режим сна
void PCA9685::sleepMode() {
    int mode1 = readRegister(MODE1);
    // Установка бита сна (бит 4)
    mode1 |= (1 << 4);
    writeRegister(MODE1, mode1);
}

// Установка частоты ШИМ
void PCA9685::setPWMFreq(uint16_t freq) {
    float prescale_val = 25000000.0; // 25MHz
    prescale_val /= 4096.0;          // 12-bit
    prescale_val /= float(freq);
    prescale_val -= 1.0;

    uint8_t prescale = floor(prescale_val + 0.5);

    uint8_t old_mode = readRegister(0x00); // Чтение MODE1 регистра
    uint8_t new_mode = (old_mode & 0x7F) | 0x10; // Sleep
    writeRegister(0x00, new_mode); // Переход в режим сна
    writeRegister(PRE_SCALE, prescale); // Установка регистра предделителя
    writeRegister(0x00, old_mode);
    usleep(5000); // Задержка для установки частоты
    writeRegister(0x00, old_mode | 0x80); // Включение
}

// Метод для получения текущей частоты ШИМ
float PCA9685::getPWMFreq() {
    uint8_t prescale = readRegister(PRE_SCALE);
    float freq = 25000000.0f; // Осцилляторная частота 25 МГц
    freq /= 4096.0f; // Разрешение ШИМ 12 бит
    freq /= (prescale + 1); // Добавляем 1 к предделителю
    return freq;
}

// Установка скважности ШИМ
void PCA9685::setPWMDutyCycle(uint8_t channel, uint16_t on, uint16_t off) {
    if(channel < 16) {
        writeRegister(LED0_ON_L + 4 * channel, on & 0xFF);
        writeRegister(LED0_ON_H + 4 * channel, on >> 8);
        writeRegister(LED0_OFF_L + 4 * channel, off & 0xFF);
        writeRegister(LED0_OFF_H + 4 * channel, off >> 8);
    }
}

// Метод для получения скважности ШИМ для определенного канала
uint16_t PCA9685::getPWMDutyCycle(uint8_t channel) {
    if (channel < 0 || channel > 15) {
        std::cerr << "Channel number out of range (0-15)." << std::endl;
        return -1;
    }

    int onValue = readRegister(LED0_ON_L + 4 * channel) |
                 (readRegister(LED0_ON_H + 4 * channel) << 8);
    int offValue = readRegister(LED0_OFF_L + 4 * channel) |
                  (readRegister(LED0_OFF_H + 4 * channel) << 8);

    // Расчет скважности
    int dutyCycle = offValue - onValue;
    return dutyCycle;
}

// Функция для записи в регистр
void PCA9685::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if (write(file, buf, 2) != 2) {
        // Обработка ошибки записи
    }
}

// Функция для чтения из регистра
uint8_t PCA9685::readRegister(uint8_t reg) {
    write(file, &reg, 1); // Установка адреса регистра для чтения
    uint8_t value;
    if (read(file, &value, 1) != 1) {
        // Обработка ошибки чтения
    }
    return value;
}


// int main() {
//     // Пример использования
//     PCA9685 pca9685(1, 0x40); // Указываем номер шины и адрес устройства
//     pca9685.setPWMFreq(1000); // Установка частоты ШИМ
//     pca9685.setPWMDutyCycle(0, 0, 2048); // Установка скважности для канала 0

//     return 0;
// }
