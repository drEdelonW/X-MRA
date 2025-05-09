#include "PA_PCA9685.hpp"

#include <iomanip>   //std::setfill
#include <bitset>    //std::bitset

#include <unistd.h>  //usleep

#include <cmath>     //round floor

#define MAX_VALUE (4095)

PA_PCA9685::PA_PCA9685(uint8_t bus, uint8_t address) :
    i2c_bus(bus),
    i2c_address(address),
    channelInversion_{false} {
    _busInit();
    printStatus();
}

PA_PCA9685::~PA_PCA9685() {
    _busDeinit();
}

void PA_PCA9685::wakeUp() {
    PCA_Register mode1 = _readRegister(MODE1);
    mode1 &= ~(1 << 4);    // Сброс бита сна (бит 4)
    _writeRegister(MODE1, mode1);
    usleep(500);    // Ждем 500 микросекунд, чтобы убедиться в стабилизации осциллятора
}

void PA_PCA9685::sleepMode() {
    PCA_Register mode1 = _readRegister(MODE1);
    mode1 |= (1 << 4);  // Установка бита сна (бит 4)
    _writeRegister(MODE1, mode1);
}

Hertz PA_PCA9685::getRealFrequencyHz(Hertz desiredFreq) {
    float oscFreq = 25000000.0; // Осцилляторная частота PA_PCA9685 в Гц (25 МГц)
    float prescaleVal = oscFreq / (4096 * desiredFreq) - 1;
    int prescale = std::round(prescaleVal); // Округляем до ближайшего целого
    // Обратное преобразование для получения реальной частоты
    float realFreq = oscFreq / (4096 * (prescale + 1));

    return realFreq;
}

void PA_PCA9685::setFreq_Hz(Hertz freq) {
    Hertz prescale_val = 25000000.0;
    prescale_val /= 4096.0;
    prescale_val /= float(freq);
    prescale_val -= 1.0;

    PCA_Register prescale = floor(prescale_val + 0.5);

    // Enter sleep
    PCA_Register old_mode = _readRegister(MODE1);
    PCA_Register sleep_mode = (old_mode & 0x7F) | 0x10;
    _writeRegister(MODE1, sleep_mode);
    usleep(500); // Wait for oscillator to stop

    // Set prescale
    _writeRegister(PRE_SCALE, prescale);

    // Exit sleep
    _writeRegister(MODE1, old_mode);   // Clear sleep
    usleep(500); // Allow oscillator to stabilize

    // Restart (if auto-increment etc. is needed)
    _writeRegister(MODE1, old_mode | 0x80);
}

float PA_PCA9685::getFreq_Hz() {
    PCA_Register prescale = _readRegister(PRE_SCALE);
    float freq = 25000000.0f; // Осцилляторная частота 25 МГц
    freq /= 4096.0f; // Разрешение ШИМ 12 бит
    freq /= (prescale + 1); // Добавляем 1 к предделителю
    return freq;
}

MicroSeconds PA_PCA9685::calcUnitDurationUs() {
    float period = 1 / getFreq_Hz(); // Период ШИМ в секундах
    float unitWeight = (period / 4096) * 1000000; // Вес единицы в микросекундах

    return static_cast<int>(std::round(unitWeight));    // Округляем результат до ближайшего целого числа
}

void PA_PCA9685::setDutyCycle(uint8_t channel, uint16_t duration, uint16_t phaseShift) {
    duration = duration % 4096;
    phaseShift = phaseShift % 4096;

    uint16_t on = phaseShift;   // Вычисляем значения on и off, учитывая сдвиг фазы
    uint16_t off = (on + duration) % 4096; // Учитываем переполнение
    
    if (channelInversion_[channel]) {
        std::swap(on, off);    // Инвертируем сигнал для данного канала
    }
    
    if (channel < LED_NUM) {
        _writeRegister(LED0_ON_L  + 4 * channel, on & 0xFF);
        _writeRegister(LED0_ON_H  + 4 * channel, on >> 8);
        _writeRegister(LED0_OFF_L + 4 * channel, off & 0xFF);
        _writeRegister(LED0_OFF_H + 4 * channel, off >> 8);
    }
}

uint16_t PA_PCA9685::getDutyCycle(uint8_t channel) {
    if (!(channel < LED_NUM)) {
        fprintf(stderr, "Error: Channel number out of range (0-15).\n");
        return -1;
    }
    

    int onValue     = _readRegister(LED0_ON_L  + 4 * channel) |
                     (_readRegister(LED0_ON_H  + 4 * channel) << 8);
    int offValue    = _readRegister(LED0_OFF_L + 4 * channel) |
                     (_readRegister(LED0_OFF_H + 4 * channel) << 8);

    int dutyCycle;
    if (offValue >= onValue) {
        dutyCycle = offValue - onValue;
    } else {
        // Учитываем закольцованность значений
        dutyCycle = (4096 - onValue) + offValue;
    }
    return dutyCycle;
}

void PA_PCA9685::setInversion(uint8_t channel, bool inverted) {
    if (channel < LED_NUM) {
        channelInversion_[channel] = inverted;
    }
}

bool PA_PCA9685::getInversion(uint8_t channel) const {
    if (channel < LED_NUM) {
        return channelInversion_[channel];
    }
    return false;
}

uint16_t PA_PCA9685::getMaxValue() const {
    return MAX_VALUE;
}
