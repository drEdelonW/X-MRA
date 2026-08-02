#pragma once
#include "proto_PWM.hpp"

typedef enum : uint8_t {
    PwmCh0, PwmCh1, PwmCh2, PwmCh3, PwmCh4,
    PwmCh5, PwmCh6, PwmCh7, PwmCh8, PwmCh9,

    PwmCh10 = 10, PwmChA = 10,
    PwmCh11 = 11, PwmChB = 11,
    PwmCh12 = 12, PwmChC = 12,
    PwmCh13 = 13, PwmChD = 13,
    PwmCh14 = 14, PwmChE = 14,
    PwmCh15 = 15, PwmChF = 15,

    PwmChNum  /* Number of LED channels */
} PwmChannel;

class ProtoPWMArray {
  public:
    virtual ~ProtoPWMArray() {}

    virtual void wakeUp() = 0;
    virtual void sleepMode() = 0;

    virtual Hertz getRealFrequencyHz(Hertz desiredFreq) = 0;
    // virtual MicroSeconds calcUnitDurationUs() = 0;

    virtual  void setFreq_Hz(Hertz freq) = 0;
    virtual Hertz getFreq_Hz() = 0;

    virtual        void setDutyCycle(PwmChannel channel, DutyCycle_t dutyCycle, DutyCycle_t phaseShift = 0) = 0;
    virtual DutyCycle_t getDutyCycle(PwmChannel channel) = 0;

    virtual         void setDuration(PwmChannel channel, MicroSeconds duration, MicroSeconds phaseShift = us(0)) = 0;    // Set the PWM duty cycle
    virtual MicroSeconds getDuration(PwmChannel channel) = 0; // Method to get the PWM duty cycle for a specific channel
};

class PWMChannel : public ProtoPWM {
    ProtoPWMArray& controller;
    PwmChannel channel;
    public:
    PWMChannel(ProtoPWMArray& ctrl, PwmChannel _channel);

           void setDutyCycle(DutyCycle_t duty) override;
    DutyCycle_t getDutyCycle() override;

            void setDuration(MicroSeconds duration) override;
    MicroSeconds getDuration() override;

    MicroSeconds getPeriodUS() override;

    void disable() override;
    void enable() override;
    bool isEnabled() override;
};
