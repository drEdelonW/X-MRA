#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include <unistd.h>  //usleep
#include <cmath>     //round floor

Hertz PCA9685::getRealFrequencyHz(Hertz desiredFreq) {
    int prescale = std::round(
        Hz(OSC_FREQ_HZ) /
            (MAX_VAL * desiredFreq) - 1.f
    );

    return Hz(
        OSC_FREQ_HZ /
            (MAX_VAL * (prescale + 1))
    );
}

void PCA9685::setFreq_Hz(Hertz freq) {
    _periodUs = _getDutyCyclePeriodUs(
        _freq = getRealFrequencyHz(freq)
    );

    PCA_reg_t old_mode = _readRegister(MODE1);      // Enter sleep

    _writeRegister(MODE1, (old_mode & 0x7F) | SLEEP_BIT);   usleep(500); // Wait for oscillator to stop
    float prescale_val = Hz(OSC_FREQ_HZ) / (MAX_VAL * freq) - 1.f;
    _writeRegister(PRE_SCALE, floorf(prescale_val + 0.5f));    // Set prescale
    _writeRegister(MODE1, old_mode);  /* Exit sleep */      usleep(500); // Allow oscillator to stabilize
    _writeRegister(MODE1, old_mode | 0x80); // Restart (if auto-increment etc. is needed)
}

Hertz PCA9685::getFreq_Hz() { return _freq; }

Hertz PCA9685::_readFreq_Hz() {
    return Hz(
        OSC_FREQ_HZ /
            (MAX_VAL * (_readRegister(PRE_SCALE) + 1))
    );
}
