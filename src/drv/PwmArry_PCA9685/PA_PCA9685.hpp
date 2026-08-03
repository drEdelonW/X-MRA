#pragma once
#include "PWMChannel.hpp"
#include "i2cEndPoint.hpp"
#define  PCA_Defined

// #define  PCA_Register uint8_t   // TODO: make it enum
typedef uint8_t PCA_reg_t;
typedef enum:uint8_t {
    MODE1         = 0x00, // Mode register 1
    MODE2         = 0x01, // Mode register 2

    LED0_ON_L     = 0x06, // LED0 ON low byte
    LED0_ON_H     = 0x07, // LED0 ON high byte
    LED0_OFF_L    = 0x08, // LED0 OFF low byte
    LED0_OFF_H    = 0x09, // LED0 OFF high byte

    LED1_ON_L     = 0x0A, // LED1 ON low byte
    LED1_ON_H     = 0x0B, // LED1 ON high byte
    LED1_OFF_L    = 0x0C, // LED1 OFF low byte
    LED1_OFF_H    = 0x0D, // LED1 OFF high byte

    ALL_LED_ON_L  = 0xFA, // All LEDs ON low byte
    ALL_LED_ON_H  = 0xFB, // All LEDs ON high byte
    ALL_LED_OFF_L = 0xFC, // All LEDs OFF low byte
    ALL_LED_OFF_H = 0xFD, // All LEDs OFF high byte

    PRE_SCALE     = 0xFE  // Prescale register for frequency
} PCA_Register;

// #define PCA_Addr uint8_t   // TODO: make it enum
typedef enum : uint8_t {
  PCAaddr_0 = 0x40,
  PCAaddr_1 = 0x41 // TODO: fill according to datasheet
} PCA_Addr;

class PCA9685 : public ProtoPWMArray{
  public:
    PCA9685(i2cBus& bus, PCA_Addr address, Hertz freq = Hz(300.f));
   ~PCA9685();

    void wakeUp() override;
    void sleepMode() override;

    Hertz getRealFrequencyHz(Hertz desiredFreq) override;

    void  setFreq_Hz(Hertz freq) override;
    Hertz getFreq_Hz() override;

    PWMChannel PWM[PwmChNum];

    void        setDutyCycle(PwmChannel channel, DutyCycle_t dutyCycle, DutyCycle_t phaseShift = 0)  override;
    DutyCycle_t getDutyCycle(PwmChannel channel) override;

    void         setDuration(PwmChannel channel, MicroSeconds duration, MicroSeconds phaseShift = us(0))  override;
    MicroSeconds getDuration(PwmChannel channel) override;

    /* PCA specific API*/
    void setInversion(PwmChannel channel, bool inverted);
    bool getInversion(PwmChannel channel);

    void printStatus();

  private:
#if 0
    bool        _initted;
    int         _fd;
    uint8_t     _i2c_bus;
    uint8_t     _i2c_address;
#else
    // i2cBus&     _bus;
    i2cEndPoint _iEP;
#endif
    bool        _channelInversion[PwmChNum];

    Hertz       _freq;
    Hertz       _readFreq_Hz();

    MicroSeconds _periodUs;
    MicroSeconds _getDutyCyclePeriodUs(Hertz frequencyHz);

#if 0
    void        _busInit();
    void        _busDeinit();
#endif
    void        _writeRegister(uint8_t reg, PCA_reg_t value);    // Function to write to a register
    PCA_reg_t   _readRegister( uint8_t reg);    // Function to read from a register
};