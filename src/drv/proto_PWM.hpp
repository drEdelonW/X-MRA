#pragma once
#include <stdint.h>

using Hertz = float;
using MicroSeconds = uint32_t;
using MilliSeconds = uint32_t;
using DutyCycle = float;        // Range: 0.0 to 1.0
using Tick = uint16_t;          // Controller-specific tick unit

class ProtoPWM {
  public:
    virtual ~ProtoPWM() {}

    virtual         void setPulseDurationUS(MicroSeconds duration_us) = 0;
    virtual MicroSeconds getPulseDurationUS() const = 0;

    virtual      void setDutyCycle(DutyCycle duty) = 0;
    virtual DutyCycle getDutyCycle() const = 0;

    virtual MicroSeconds getPeriodUS() const = 0;

    virtual void getRawTicks(Tick& on_tick, Tick& off_tick) const = 0;
    virtual void setRawTicks(Tick  on_tick, Tick  off_tick) = 0;

    virtual void disable() = 0;
    virtual void enable() = 0;
    virtual bool isEnabled() const = 0;
};
    
// class protoPWM {
//     void wakeUp();  // Method to wake up the chip from sleep mode
//     void sleepMode();   // Method to put the chip into sleep mode

//     // void printStatus(); // Method to print the state of the registers

//     float getRealFrequencyHz(float desiredFreq);    // Method to calculate the real PWM frequency based on the desired frequency
//     int   calcUnitDurationUs();   // Method to calculate the weight of a unit in microseconds based on the current pre-scale value

//     void  setFreq_Hz(uint16_t freq);    // Set the PWM frequency
//     float getFreq_Hz();                 // Method to get the current PWM frequency


//     void     setDutyCycle(uint8_t channel, uint16_t duration, uint16_t phaseShift = 0) ;    // Set the PWM duty cycle
//     uint16_t getDutyCycle(uint8_t channel); // Method to get the PWM duty cycle for a specific channel

//     void setInversion(uint8_t channel, bool inverted);
//     bool getInversion(uint8_t channel) const;

//     // uint16_t getMaxValue() const;

// };