#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"

#include <unistd.h>  //usleep

#include <cmath>     //round floor


Hertz PCA9685::getRealFrequencyHz(Hertz desiredFreq) {
    float oscFreq = OSC_FREQ_HZ;
    float prescaleVal = oscFreq / (MAX_VAL * desiredFreq) - 1;
    int prescale = std::round(prescaleVal); // Округляем до ближайшего целого

    // Обратное преобразование для получения реальной частоты
    float realFreq = oscFreq / (MAX_VAL * (prescale + 1));

    return realFreq;
}

void PCA9685::setFreq_Hz(Hertz freq) {
    _freq = getRealFrequencyHz(freq);
    _periodUs = _getDutyCyclePeriodUs(_freq);
    Hertz prescale_val = (
        (OSC_FREQ_HZ / MAX_VAL) /
        float(freq)
    ) - 1.0 ;

    PCA_Register prescale = floor(prescale_val + 0.5);

    PCA_Register old_mode = _readRegister(MODE1);
    // Enter sleep
    PCA_Register sleep_mode = (old_mode & 0x7F) | 0x10;
    _writeRegister(MODE1, sleep_mode);
    usleep(500); // Wait for oscillator to stop

    // Set prescale
    _writeRegister(PRE_SCALE, prescale);

    // Exit sleep
    _writeRegister(MODE1, old_mode);   // Clear sleep
    usleep(500); // Allow oscillator to stabilize

    // Restart (if auto-increment etc. is needed)
    _writeRegister(MODE1, old_mode | 0x80);
}

Hertz PCA9685::getFreq_Hz() {
    return _freq;
}

Hertz PCA9685::_readFreq_Hz() {
    return
        (OSC_FREQ_HZ / MAX_VAL) /
        (_readRegister(PRE_SCALE) + 1);
}
