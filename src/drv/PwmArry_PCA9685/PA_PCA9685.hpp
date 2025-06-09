#pragma once
#include "PWMChannel.hpp"
#define  PCA_Defined

#define  PCA_Register uint8_t

class PCA9685 : public ProtoPWMArray{
  public:
    PCA9685(uint8_t bus, uint8_t address);
   ~PCA9685();

    void wakeUp() override;
    void sleepMode() override;

    Hertz getRealFrequencyHz(Hertz desiredFreq) override;

    void  setFreq_Hz(Hertz freq) override;
    Hertz getFreq_Hz() override;

    PWMChannel PWM[16];

    void        setDutyCycle(uint8_t channel, DutyCycle dutyCycle, DutyCycle phaseShift = 0)  override;
    DutyCycle   getDutyCycle(uint8_t channel) override;

    void         setDuration(uint8_t channel, MicroSeconds duration, MicroSeconds phaseShift = 0)  override;
    MicroSeconds getDuration(uint8_t channel) override;

    void setInversion(uint8_t channel, bool inverted);
    bool getInversion(uint8_t channel);

    void printStatus();

  private:
    bool        _initted;
    int         _fd;
    uint8_t     _i2c_bus;
    uint8_t     _i2c_address;
    bool        _channelInversion[16];

    Hertz       _freq;
    Hertz       _readFreq_Hz();

    MicroSeconds _periodUs;
    MicroSeconds _getDutyCyclePeriodUs(Hertz frequencyHz);

    void         _busInit();
    void         _busDeinit();
    void         _writeRegister(PCA_Register reg, uint8_t value);    // Function to write to a register
    uint8_t      _readRegister( PCA_Register reg);    // Function to read from a register
};