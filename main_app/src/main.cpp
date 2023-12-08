#include <stdio.h>
#include <unistd.h> 

// #include "x11.h"
// #include "i2c.h"

#include "pca9685_obj.h"

int main() {
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();

    PCA9685 PWM(1, 0x40); // Указываем номер шины и адрес устройства
    // printf("base freq %f\n", PWM.getPWMFreq());
    PWM.setPWMFreq(50); // Установка частоты ШИМ
    printf("new freq %f\n", PWM.getPWMFreq());
    PWM.wakeUp();
    for(int i = 205; i < 410; i++){
        // printf("duty %d\n", PWM.getPWMDutyCycle(0));
        usleep(10000);
        PWM.setPWMDutyCycle(0, 0, i); // Установка скважности для канала 0
        PWM.setPWMDutyCycle(1, 0, i); // Установка скважности для канала 0
    }

    usleep(1000 * 1000);

    PWM.setPWMFreq(100); // Установка частоты ШИМ
    printf("new freq %f\n", PWM.getPWMFreq());
    PWM.wakeUp();
    for(int i = 409; i < 819; i++){
        usleep(5000);
        PWM.setPWMDutyCycle(0, 0, i); // Установка скважности для канала 0
        PWM.setPWMDutyCycle(1, 0, i); // Установка скважности для канала 0
    }

    usleep(1000 * 1000);

    PWM.setPWMFreq(150); // Установка частоты ШИМ
    printf("new freq %f\n", PWM.getPWMFreq());
    PWM.wakeUp();
    for(int i = 614; i < 1229; i++){
        usleep(3000);
        PWM.setPWMDutyCycle(0, 0, i); // Установка скважности для канала 0
        PWM.setPWMDutyCycle(1, 0, i); // Установка скважности для канала 0
    }

    usleep(1000 * 1000);

    PWM.setPWMFreq(200); // Установка частоты ШИМ
    printf("new freq %f\n", PWM.getPWMFreq());
    PWM.wakeUp();
    for(int i = 819; i < 1638; i++){
        usleep(2500);
        PWM.setPWMDutyCycle(0, 0, i); // Установка скважности для канала 0
        PWM.setPWMDutyCycle(1, 0, i); // Установка скважности для канала 0
    }

    PWM.sleepMode();

    // PWM.printStatus();

    return 0;
}
