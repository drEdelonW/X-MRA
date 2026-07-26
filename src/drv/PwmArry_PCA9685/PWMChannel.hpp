#pragma once
#include "proto_PWM.hpp"

class ProtoPWMArray {
  public:
    virtual ~ProtoPWMArray() {}

    virtual void wakeUp() = 0;
    virtual void sleepMode() = 0;

    virtual Hertz getRealFrequencyHz(Hertz desiredFreq) = 0;
    // virtual MicroSeconds calcUnitDurationUs() = 0;

    virtual  void setFreq_Hz(Hertz freq) = 0;
    virtual Hertz getFreq_Hz() = 0;

    virtual        void setDutyCycle(uint8_t channel, DutyCycle_t dutyCycle, DutyCycle_t phaseShift = 0) = 0;
    virtual DutyCycle_t getDutyCycle(uint8_t channel) = 0;

    virtual         void setDuration(uint8_t channel, MicroSeconds duration, MicroSeconds phaseShift = us(0)) = 0;    // Set the PWM duty cycle
    virtual MicroSeconds getDuration(uint8_t channel) = 0; // Method to get the PWM duty cycle for a specific channel
};

class PWMChannel : public ProtoPWM {
    ProtoPWMArray& controller;
    uint8_t channel;
    public:
    PWMChannel(ProtoPWMArray& ctrl, uint8_t ch);

           void setDutyCycle(DutyCycle_t duty) override;
    DutyCycle_t getDutyCycle() override;

            void setDuration(MicroSeconds duration) override;
    MicroSeconds getDuration() override;

    MicroSeconds getPeriodUS() override;

    void disable() override;
    void enable() override;
    bool isEnabled() override;
};
