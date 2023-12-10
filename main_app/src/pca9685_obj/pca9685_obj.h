#pragma once 
#include <stdint.h>
#include <array>

#define def(x,v)    static constexpr int x = v;
#define REG_TYPE    uint8_t
class PCA9685 {
public:
    PCA9685(uint8_t bus, uint8_t address);
   ~PCA9685();
    
    // Method to print the state of the registers
    void printStatus();

    // Method to wake up the chip from sleep mode
    void wakeUp();

    // Method to put the chip into sleep mode
    void sleepMode();

    // Method to calculate the real PWM frequency based on the desired frequency
    float getRealFrequencyHz(float desiredFreq);

    // Set the PWM frequency
    void setFreq_Hz(uint16_t freq);

    // Method to get the current PWM frequency
    float getFreq_Hz();

    // Method to calculate the weight of a unit in microseconds based on the current pre-scale value
    int calcUnitDurationUs();

    // Set the PWM duty cycle
    void setDutyCycle(uint8_t channel, uint16_t duration, uint16_t phaseShift = 0) ;

    // Method to get the PWM duty cycle for a specific channel
    uint16_t getDutyCycle(uint8_t channel);

    void setInversion(uint8_t channel, bool inverted);

    bool getInversion(uint8_t channel) const;

    uint16_t getMaxValue() const;
    
private:
    static constexpr uint16_t MAX_VALUE = 4095; // Максимальное значение для 12-битного ШИМ
    // Constants for register addresses
    def(MODE1,          0x00);  // static constexpr int MODE1 = 0x00;
    def(MODE2,          0x01);  // static constexpr int MODE2 = 0x01;

    def(LED_NUM,        0x10);  // static constexpr int LED_NUM = 0x10;
    def(LED0_ON_L,      0x06);  // static constexpr int LED0_ON_L = 0x06;
    def(LED0_ON_H,      0x07);  // static constexpr int LED0_ON_H = 0x07;
    def(LED0_OFF_L,     0x08);  // static constexpr int LED0_OFF_L = 0x08;
    def(LED0_OFF_H,     0x09);  // static constexpr int LED0_OFF_H = 0x09;

    def(ALL_LED_ON_L,   0xFA);  // static constexpr int ALL_LED_ON_L = 0xFA;
    def(ALL_LED_ON_H,   0xFB);  // static constexpr int ALL_LED_ON_H = 0xFB;
    def(ALL_LED_OFF_L,  0xFC);  // static constexpr int ALL_LED_OFF_L = 0xFC;
    def(ALL_LED_OFF_H,  0xFD);  // static constexpr int ALL_LED_OFF_H = 0xFD;

    def(PRE_SCALE,      0xFE);  // static constexpr int PRE_SCALE = 0xFE;

    int fd;
    uint8_t i2c_bus;
    uint8_t i2c_address;
    std::array<bool, LED_NUM> channelInversion_; // Флаги инверсии для каждого канала

    // Function to write to a register
    void _writeRegister(uint8_t reg, REG_TYPE value);

    // Function to read from a register
    REG_TYPE _readRegister(uint8_t reg);
};