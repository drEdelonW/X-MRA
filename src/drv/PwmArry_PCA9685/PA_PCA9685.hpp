#pragma once
#include "PWMChannel.hpp"
#define  PCA_Defined

#define  PCA_Register uint8_t   // TODO: make it enum

typedef enum : uint8_t {
    PwmCh0, PwmCh1, PwmCh2, PwmCh3, PwmCh4,
    PwmCh5, PwmCh6, PwmCh7, PwmCh8, PwmCh9,

    PwmCh10 = 10, PwmChA = 10,
    PwmCh11 = 11, PwmChB = 11,
    PwmCh12 = 12, PwmChC = 12,
    PwmCh13 = 13, PwmChD = 13,
    PwmCh14 = 14, PwmChE = 14,
    PwmCh15 = 15, PwmChF = 15,

    PwmChNum
} PwmChannel;

// #define PCA_Addr uint8_t   // TODO: make it enum
typedef enum : uint8_t {
  PCAaddr_0 = 0x40,
  PCAaddr_1 = 0x41 // TODO: fill according to datasheet
} PCA_Addr;

class PCA9685 : public ProtoPWMArray{
  public:
    PCA9685(uint8_t bus, PCA_Addr address, Hertz freq = Hz(300.f));
   ~PCA9685();

    void wakeUp() override;
    void sleepMode() override;

    Hertz getRealFrequencyHz(Hertz desiredFreq) override;

    void  setFreq_Hz(Hertz freq) override;
    Hertz getFreq_Hz() override;

    PWMChannel PWM[PwmChNum];

    void        setDutyCycle(uint8_t channel, DutyCycle_t dutyCycle, DutyCycle_t phaseShift = 0)  override;
    DutyCycle_t getDutyCycle(uint8_t channel) override;

    void         setDuration(uint8_t channel, MicroSeconds duration, MicroSeconds phaseShift = us(0))  override;
    MicroSeconds getDuration(uint8_t channel) override;

    /* PCA speecific API*/
    void setInversion(uint8_t channel, bool inverted);
    bool getInversion(uint8_t channel);

    void printStatus();

  private:
    bool        _initted;
    int         _fd;
    uint8_t     _i2c_bus;
    uint8_t     _i2c_address;
    bool        _channelInversion[PwmChNum];

    Hertz       _freq;
    Hertz       _readFreq_Hz();

    MicroSeconds _periodUs;
    MicroSeconds _getDutyCyclePeriodUs(Hertz frequencyHz);

    void         _busInit();
    void         _busDeinit();
    void         _writeRegister(PCA_Register reg, uint8_t value);    // Function to write to a register
    uint8_t      _readRegister( PCA_Register reg);    // Function to read from a register
};