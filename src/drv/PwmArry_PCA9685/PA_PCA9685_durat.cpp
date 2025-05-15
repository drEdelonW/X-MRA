#include "PA_PCA9685.hpp"
#include "PA_PCA9685_private.hpp"
#include "terminal_tools.h"

#include <cmath>     //round floor

// MicroSeconds PCA9685::calcUnitDurationUs() {
//     float period = 1 / getFreq_Hz(); // Период ШИМ в секундах
//     float unitWeight = (period / MAX_VAL) * 1000000; // Вес единицы в микросекундах

//     return static_cast<int>(std::round(unitWeight));    // Округляем результат до ближайшего целого числа
// }


MicroSeconds PCA9685::_getDutyCyclePeriodUs(Hertz frequencyHz) {
    if (frequencyHz <= 0.0f)
        return 0.0f;
    return 1000000.0f / frequencyHz;
}

void PCA9685::setDuration(uint8_t channel, MicroSeconds duration, MicroSeconds phaseShift){
    // LOG("setDur frq[%f]Hz dur[%ld]us per[%ld]us\n", _freq, duration, _periodUs);
    setDutyCycle(channel, (1.0f * duration) / _periodUs );
    // setDutyCycle(channel, /*(1.0f * duration) / _periodUs*/0.1 );
    // setDutyCycle(channel, /*(1.0f * duration) / _periodUs*/0.05 );
}

MicroSeconds PCA9685::getDuration(uint8_t channel){ return 0;}

