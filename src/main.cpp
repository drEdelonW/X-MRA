#include "CLI.h"

// #include "x11.h"
// #include "robot_spec.hpp"
#include "joystick.hpp"
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
    GameControllerInit();
    startCLI();
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();
    // testLegs();

    return 0;
}
