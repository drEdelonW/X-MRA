#pragma once
#include "PWMChannel.hpp"
// #define PWM_ARRAY PCA9685

#define  PCA_Register uint8_t

class PCA9685 : public ProtoPWMArray{
public:
    PCA9685(uint8_t bus, uint8_t address);
   ~PCA9685();

    void wakeUp() override;      // Method to wake up the chip from sleep mode
    void sleepMode() override;   // Method to put the chip into sleep mode

    void printStatus(); // Method to print the state of the registers

    Hertz getRealFrequencyHz(Hertz desiredFreq) override;    // Method to calculate the real PWM frequency based on the desired frequency
    MicroSeconds   calcUnitDurationUs() override;   // Method to calculate the weight of a unit in microseconds based on the current pre-scale value

    void  setFreq_Hz(Hertz freq) override;    // Set the PWM frequency
    Hertz getFreq_Hz() override;                 // Method to get the current PWM frequency

    PWMChannel PWM[16];

           void  setDutyCycle(uint8_t channel, MicroSeconds duration, MicroSeconds phaseShift = 0)  override;    // Set the PWM duty cycle
    MicroSeconds getDutyCycle(uint8_t channel) override; // Method to get the PWM duty cycle for a specific channel

    void setInversion(uint8_t channel, bool inverted);
    bool getInversion(uint8_t channel);

    uint16_t getMaxValue();

private:
    int         fd;
    uint8_t     i2c_bus;
    uint8_t     i2c_address;
    bool        channelInversion_[16];

    void         _busInit();
    void         _busDeinit();
    void         _writeRegister(PCA_Register reg, uint8_t value);    // Function to write to a register
    uint8_t      _readRegister( PCA_Register reg);    // Function to read from a register
};