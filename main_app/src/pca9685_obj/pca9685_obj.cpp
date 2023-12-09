#include "pca9685_obj.h"

#include <iostream>
#include <iomanip>
#include <bitset>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>

PCA9685::PCA9685(uint8_t bus, uint8_t address) : i2c_bus(bus), i2c_address(address) {
    // Открытие шины I2C
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", bus);
    fd = open(filename, O_RDWR);

    if (fd < 0) {
        // Обработка ошибки открытия файла
    }

    // Установка адреса устройства
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        // Обработка ошибки соединения
    }
}

PCA9685::~PCA9685() {
    close(fd);
}

void PCA9685::printStatus() {
    REG_TYPE mode1      = _readRegister(MODE1);
    REG_TYPE mode2      = _readRegister(MODE2);
    REG_TYPE prescale   = _readRegister(PRE_SCALE);
    
    float oscFreq = 25000000.0; // Осцилляторная частота в Гц
    float pwmFreq = oscFreq / (4096.0 * (prescale + 1)); // Расчет частоты ШИМ

    std::cout   << "Расширенное состояние PCA9685:" << std::endl;
    std::cout   << "MODE1: " << std::bitset<8>(mode1)
                    << " (режим сна: "
                        << ((mode1 & 0x10) ? "включен" : "выключен")
                    << ")"
                << std::endl;
    std::cout   << "MODE2: " << std::bitset<8>(mode2) << std::endl;
    std::cout   << "PRE_SCALE: "
                    << prescale << " (Приблизительная частота ШИМ: " << pwmFreq << " Гц)"
                << std::endl;


    for (int channel = 0; channel < 16; ++channel) {
        int onValue  = _readRegister(LED0_ON_L + 4 * channel) | (_readRegister(LED0_ON_H + 4 * channel) << 8);
        int offValue = _readRegister(LED0_OFF_L + 4 * channel) | (_readRegister(LED0_OFF_H + 4 * channel) << 8);

        std::cout   << "Канал "     << std::setw(2) << std::setfill('0') << channel 
                    << ": ON = "    << std::setw(4) << std::setfill('0') << onValue 
                    << ", OFF = "   << std::setw(4) << std::setfill('0') << offValue
                    << std::endl;
    }
}

void PCA9685::wakeUp() {
    REG_TYPE mode1 = _readRegister(MODE1);
    mode1 &= ~(1 << 4);    // Сброс бита сна (бит 4)
    _writeRegister(MODE1, mode1);
    usleep(500);    // Ждем 500 микросекунд, чтобы убедиться в стабилизации осциллятора
}


void PCA9685::sleepMode() {
    REG_TYPE mode1 = _readRegister(MODE1);
    mode1 |= (1 << 4);  // Установка бита сна (бит 4)
    _writeRegister(MODE1, mode1);
}


float PCA9685::getRealFrequencyHz(float desiredFreq) {
    float oscFreq = 25000000.0; // Осцилляторная частота PCA9685 в Гц (25 МГц)
    float prescaleVal = oscFreq / (4096 * desiredFreq) - 1;
    int prescale = std::round(prescaleVal); // Округляем до ближайшего целого
    // Обратное преобразование для получения реальной частоты
    float realFreq = oscFreq / (4096 * (prescale + 1));

    return realFreq;
}

void PCA9685::setFreq_Hz(uint16_t freq) {
    float prescale_val = 25000000.0; // 25MHz
    prescale_val /= 4096.0;          // 12-bit
    prescale_val /= float(freq);
    prescale_val -= 1.0;

    REG_TYPE prescale = floor(prescale_val + 0.5);
    REG_TYPE old_mode = _readRegister(0x00);    // Чтение MODE1 регистра
    REG_TYPE new_mode = (old_mode & 0x7F) | 0x10;   // Sleep
    _writeRegister(0x00, new_mode);         // Переход в режим сна
    _writeRegister(PRE_SCALE, prescale);    // Установка регистра предделителя
    _writeRegister(0x00, old_mode);
    usleep(5000);       // Задержка для установки частоты
    _writeRegister(0x00, old_mode | 0x80);  // Включение
}

float PCA9685::getFreq_Hz() {
    REG_TYPE prescale = _readRegister(PRE_SCALE);
    float freq = 25000000.0f; // Осцилляторная частота 25 МГц
    freq /= 4096.0f; // Разрешение ШИМ 12 бит
    freq /= (prescale + 1); // Добавляем 1 к предделителю
    return freq;
}

int PCA9685::calcUnitDurationUs() {
    float period = 1 / getFreq_Hz(); // Период ШИМ в секундах
    float unitWeight = (period / 4096) * 1000000; // Вес единицы в микросекундах

    return static_cast<int>(std::round(unitWeight));    // Округляем результат до ближайшего целого числа
}

void PCA9685::setDutyCycle(uint8_t channel, uint16_t on, uint16_t off) {
    if(channel < LED_NUM) {
        _writeRegister(LED0_ON_L  + 4 * channel, on & 0xFF);
        _writeRegister(LED0_ON_H  + 4 * channel, on >> 8);
        _writeRegister(LED0_OFF_L + 4 * channel, off & 0xFF);
        _writeRegister(LED0_OFF_H + 4 * channel, off >> 8);
    }
}


uint16_t PCA9685::getDutyCycle(uint8_t channel) {
    if(!(channel < LED_NUM)) {
        std::cerr << "Channel number out of range (0-15)." << std::endl;
        return -1;
    }

    int onValue     = _readRegister(LED0_ON_L  + 4 * channel) |
                     (_readRegister(LED0_ON_H  + 4 * channel) << 8);
    int offValue    = _readRegister(LED0_OFF_L + 4 * channel) |
                     (_readRegister(LED0_OFF_H + 4 * channel) << 8);

    int dutyCycle = offValue - onValue; // Расчет скважности
    return dutyCycle;
}


void PCA9685::_writeRegister(uint8_t reg, REG_TYPE value) {
    REG_TYPE buf[2] = {reg, value};
    if (write(fd, buf, 2) != 2) {
        // Обработка ошибки записи
    }
}


REG_TYPE PCA9685::_readRegister(uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {  // Установка адреса регистра для чтения
        // Обработка ошибки чтения
    };
    REG_TYPE value;
    if (read(fd, &value, 1) != 1) {
        // Обработка ошибки чтения
    }
    return value;
}

