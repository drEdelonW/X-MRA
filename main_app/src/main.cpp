#include <stdio.h>
#include <unistd.h> 

// #include "x11.h"
// #include "i2c.h"

// #include "pca9685_obj.h"
// #include "pwm_servo_obj.h"
#include "hexapod_leg_obj.h"

static PCA9685 PWM[2] = {
    PCA9685(1, 0x40),
    PCA9685(1, 0x41)
};

static Servo servos[9] = {
    Servo(&PWM[0], 0),
    Servo(&PWM[0], 1),
    Servo(&PWM[0], 2),

    Servo(&PWM[0], 4),
    Servo(&PWM[0], 5),
    Servo(&PWM[0], 6),

    Servo(&PWM[0], 8),
    Servo(&PWM[0], 9),
    Servo(&PWM[0], 10)
};

static HexapodLeg BR = HexapodLeg(
        &servos[0],
        &servos[1],
        &servos[2]
    );

static HexapodLeg CR = HexapodLeg(
        &servos[3],
        &servos[4],
        &servos[5]
    );

static HexapodLeg FR = HexapodLeg(
        &servos[6],
        &servos[7],
        &servos[8]
    );

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

    // usleep(1000 * 1000);
    // testSRV(100, 180, 1040, 5000);

    // usleep(1000 * 1000);
    // testSRV(150, 270, 1560, 3000);

    // usleep(1000 * 1000);
    // testSRV(200, 419, 2038, 2500);
    // PWM[0].setFreq_Hz(100);
    // PWM[0].wakeUp();
    // servos[0].setAngleDegrees(0);
    // servos[2].setAngleDegrees(0);       usleep(1000 * 1000);
    // servos[0].setAngleDegrees(90);
    // servos[1].setAngleDegrees(90);      usleep(1000 * 1000);
    // servos[1].setAngleDegrees(120);     usleep(1000 * 1000);
    // servos[1].setAngleDegrees(60);      usleep(1000 * 1000);
    // servos[2].setAngleDegrees(90);
    // usleep(1000 * 10000);
    // servos[0].deactivate();
    // servos[1].deactivate();
    // servos[2].deactivate();
    BR.setTipPosition(5.0, 5.0, 5.0);
    CR.setTipPosition(5.0, 5.0, 5.0);
    FR.setTipPosition(5.0, 5.0, 5.0);
    usleep(1000 * 3000);
    BR.setTipPosition(5.0, 5.0, 5.2);
    CR.setTipPosition(5.0, 5.0, 5.2);
    FR.setTipPosition(5.0, 5.0, 5.2);
    usleep(1000 * 3000);
    BR.setTipPosition(5.0, 5.0, 5.2);
    CR.setTipPosition(5.0, 5.0, 5.2);
    FR.setTipPosition(5.0, 5.0, 5.2);
    usleep(1000 * 3000);

    PWM[0].sleepMode();

    // PWM[0].printStatus();

    return 0;
}
