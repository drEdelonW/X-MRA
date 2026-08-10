#include "PA_ADS7830_cWrap.h"
#include "PA_ADS7830.hpp"
#include <stdio.h>

const char* chName[8] = {
    "vBat Servo",   // CH0
    "aux1",         // CH1
    "aux2",         // CH2
    "aux3",         // CH3
    "vBat Brain",   // CH4
    "Rear  legs",   // CH5 - confirmed: spikes on rear leg servo movement
    "Mid   legs",   // CH6 - confirmed: spikes on middle leg servo movement
    "Front legs",   // CH7 - hypothesis, pending test with front leg servo
};

#define ADC_VREF_V              (5.f)   /* ADS7830 reference voltage, single-ended mode */
#define ADC_MAX_COUNT           (255.f) /* 8-bit resolution, full-scale code */
#define ADC_BAT_DIVIDER_COEFF   (3.f)   /* confirmed for vBat Servo / vBat Brain only */

#define ADC_BAT_SCALE  (ADC_VREF_V * ADC_BAT_DIVIDER_COEFF / ADC_MAX_COUNT)
#define ADC_RAW_SCALE  (ADC_VREF_V / ADC_MAX_COUNT) /* no divider assumed - uncalibrated channels */

float vScale[8] = {
    ADC_BAT_SCALE, // ch0  vBat Servo - confirmed
    ADC_RAW_SCALE, // ch1  aux1       - uncalibrated, do not trust as absolute voltage
    ADC_RAW_SCALE, // ch2  aux2       - uncalibrated
    ADC_RAW_SCALE, // ch3  aux3       - uncalibrated
    ADC_BAT_SCALE, // ch4  vBat Brain - confirmed
    ADC_RAW_SCALE, // ch5  Rear legs  - uncalibrated, saturates at 255 - check for clipping
    ADC_RAW_SCALE, // ch6  Mid legs   - uncalibrated
    ADC_RAW_SCALE, // ch7  Front legs - uncalibrated
};

int adcReadAll() {
    i2cBus iBus(1, true);
    if (!iBus.isInited())
        return 1;

    ADS7830 adc(iBus);
    uint8_t values[8];
    adc.readAll(values);

    printf("ADS7830 channels:\n");
    for (uint8_t ch = 0; ch < 8; ++ch)
        printf("  [%s]: (%3u) %0.1fv\n",
            chName[ch],
            values[ch],
            values[ch] * vScale[ch]
         );

    iBus.Deinit();
    return 0;
}
