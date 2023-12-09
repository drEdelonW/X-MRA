#include <stdio.h>
#include <unistd.h> 

// #include "x11.h"
// #include "i2c.h"

#include "pca9685_obj.h"

static PCA9685 PWM[2] = {
    PCA9685(1, 0x40),
    PCA9685(1, 0x41)
};

void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us) {
    PWM[0].setFreq_Hz(freq);
    printf("new freq %f\n", PWM[0].getFreq_Hz());
    PWM[0].wakeUp();
    for(int i = min_dur; i < max_dur; i+=1){
        usleep(delay_us);
        PWM[0].setDutyCycle(0, 0, i);
        // PWM[0].setDutyCycle(1, 0, i);
    }
    PWM[0].setDutyCycle(0, 0, (min_dur + max_dur) / 2);
    printf("mid is %d\n", (min_dur + max_dur) / 2);
}

int main() {
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();
    
    // testSRV(50, 90, 520, 10000);

    usleep(1000 * 1000);
    testSRV(100, 180, 1040, 5000);

    // usleep(1000 * 1000);
    // testSRV(150, 270, 1560, 3000);

    // usleep(1000 * 1000);
    // testSRV(200, 419, 2038, 2500);
    
    usleep(1000 * 10000);
    PWM[0].sleepMode();

    // PWM[0].printStatus();

    return 0;
}
