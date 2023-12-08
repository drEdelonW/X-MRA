#pragma once 
#include <stdint.h>

class PCA9685 {
public:
    PCA9685(uint8_t bus, uint8_t address);
    ~PCA9685();
    // Метод для вывода состояния регистров
    void printStatus();

    // Метод для вывода микросхемы из режима сна
    void wakeUp();

    // Метод для перевода микросхемы в режим сна
    void sleepMode();

    // Установка частоты ШИМ
    void setPWMFreq(uint16_t freq);

    // Метод для получения текущей частоты ШИМ
    float getPWMFreq();

    // Установка скважности ШИМ
    void setPWMDutyCycle(uint8_t channel, uint16_t on, uint16_t off);

    // Метод для получения скважности ШИМ для определенного канала
    uint16_t getPWMDutyCycle(uint8_t channel);

private:

    // Константы для адресов регистров
    static constexpr int MODE1 = 0x00;
    static constexpr int MODE2 = 0x01;
    static constexpr int PRE_SCALE = 0xFE;
    static constexpr int LED0_ON_L = 0x06;
    static constexpr int LED0_ON_H = 0x07;
    static constexpr int LED0_OFF_L = 0x08;
    static constexpr int LED0_OFF_H = 0x09;


    int file;
    uint8_t i2c_bus;
    uint8_t i2c_address;

    // Функция для записи в регистр
    void writeRegister(uint8_t reg, uint8_t value);

    // Функция для чтения из регистра
    uint8_t readRegister(uint8_t reg);
};