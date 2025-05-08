#pragma once 
#include "proto_PWM.hpp"
#define PWM_ARRAY PA_PCA9685

#define  PCA_Register uint8_t
enum PCA_Register_en:PCA_Register {
    MODE1         = 0x00, // Mode register 1
    MODE2         = 0x01, // Mode register 2

    LED_NUM       = 0x10, // Number of LED channels

    LED0_ON_L     = 0x06, // LED0 ON low byte
    LED0_ON_H     = 0x07, // LED0 ON high byte
    LED0_OFF_L    = 0x08, // LED0 OFF low byte
    LED0_OFF_H    = 0x09, // LED0 OFF high byte

    ALL_LED_ON_L  = 0xFA, // All LEDs ON low byte
    ALL_LED_ON_H  = 0xFB, // All LEDs ON high byte
    ALL_LED_OFF_L = 0xFC, // All LEDs OFF low byte
    ALL_LED_OFF_H = 0xFD, // All LEDs OFF high byte

    PRE_SCALE     = 0xFE  // Prescale register for frequency
};

class PA_PCA9685 {
public:
    PA_PCA9685(uint8_t bus, uint8_t address);
   ~PA_PCA9685();

    void wakeUp();      // Method to wake up the chip from sleep mode
    void sleepMode();   // Method to put the chip into sleep mode

    void printStatus(); // Method to print the state of the registers
    
    Hertz getRealFrequencyHz(Hertz desiredFreq);    // Method to calculate the real PWM frequency based on the desired frequency
    MicroSeconds   calcUnitDurationUs();   // Method to calculate the weight of a unit in microseconds based on the current pre-scale value

    void  setFreq_Hz(Hertz freq);    // Set the PWM frequency
    Hertz getFreq_Hz();                 // Method to get the current PWM frequency


    void     setDutyCycle(uint8_t channel, uint16_t duration, uint16_t phaseShift = 0) ;    // Set the PWM duty cycle
    uint16_t getDutyCycle(uint8_t channel); // Method to get the PWM duty cycle for a specific channel

    void setInversion(uint8_t channel, bool inverted);
    bool getInversion(uint8_t channel) const;

    uint16_t getMaxValue() const;

private:
    int         fd;
    uint8_t     i2c_bus;
    uint8_t     i2c_address;
    bool channelInversion_[16];

    void         _busInit();
    void         _busDeinit();
    void         _writeRegister(uint8_t reg, PCA_Register value);    // Function to write to a register
    PCA_Register _readRegister(uint8_t reg);    // Function to read from a register
};