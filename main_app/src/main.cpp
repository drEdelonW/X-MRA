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
// #include "sdl_joystick.h"
#include "common_tools.h"



int main(int argc, char *argv[]) {
#if 0
    ver_info();
    printf("Arg count is %d\n", argc);
    for(int i = 0; i < argc; ++i) {
        printf("Arg[%d] is \"%s\"\n", i, argv[i]);
    }
    main_joy(argc, argv);

    if (argc < 2) {
        return 0;
    } else {
        switch (argv[1][0]) {
            // case 's' : case 'S': return main_srv();
            // case 'c' : case 'C': return main_cli();
            case 'j' : case 'J': return main_joy(argc, argv);
        }
    }
#endif
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
