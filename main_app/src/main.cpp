#include <stdio.h>
#include <unistd.h> 

// #include "x11.h"
// #include "i2c.h"

#include "pca9685_obj.h"

static PCA9685 PWM(1, 0x40);

void testSRV(uint16_t freq, uint16_t min_dur, uint16_t max_dur, uint32_t delay_us) {
    PWM.setFreq_Hz(freq);
    printf("new freq %f\n", PWM.getFreq_Hz());
    PWM.wakeUp();
    for(int i = min_dur; i < max_dur; i+=1){
        usleep(delay_us);
        PWM.setDutyCycle(0, 0, i);
        // PWM.setDutyCycle(1, 0, i);
    }
}

int main() {
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();
    
    // testSRV(50, 90, 520, 10000);

    // usleep(1000 * 1000);
    // testSRV(100, 180, 1040, 5000);

    usleep(1000 * 1000);
    testSRV(150, 270, 1560, 3000);

    // usleep(1000 * 1000);
    // testSRV(200, 419, 2038, 2500);
    
    PWM.sleepMode();

    // PWM.printStatus();

    return 0;
}
