#include <stdio.h>
#include <unistd.h> 

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>

// #include "x11.h"
// #include "i2c.h"
#include "robot_spec.h"


int main(int argc, char *argv[]) {
    printf("Number of arguments: %d\n", argc);
    for(int i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();
    testLegs();

    // int fd = open("/dev/hidraw0", O_RDONLY);
    // if (fd < 0) {
    //     std::cerr << "Не удалось открыть устройство" << std::endl;
    //     return 1;
    // }

    // struct hidraw_devinfo info;
    // if (ioctl(fd, HIDIOCGRAWINFO, &info) < 0) {
    //     std::cerr << "Ошибка при получении информации о дескрипторе устройства" << std::endl;
    //     close(fd);
    //     return 1;
    // }

    // std::cout << "Вендор: " << info.vendor << " Продукт: " << info.product << std::endl;

    // close(fd);
    return 0;
}
