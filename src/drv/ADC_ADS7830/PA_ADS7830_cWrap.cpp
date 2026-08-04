#include "PA_ADS7830_cWrap.h"
#include "PA_ADS7830.hpp"
#include <stdio.h>

int adcReadAll() {
    i2cBus iBus(1, true);
    if (!iBus.isInited())
        return 1;

    ADS7830 adc(iBus);
    uint8_t values[8];
    adc.readAll(values);

    printf("ADS7830 channels:\n");
    for (uint8_t ch = 0; ch < 8; ++ch)
        printf("  CH%u: %3u\n", ch, values[ch]);

    iBus.Deinit();
    return 0;
}
