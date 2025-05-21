#include "CLI.h"
#include "dpad.h"

#include <csignal>

#include "common_tools.h"

#include "robot_spec.hpp"


void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("Received SIGINT (Ctrl+C). Exiting...\n"); fflush(stdout);
        // posix_socket.Close();
        XMRA.DISARM();
        restoreTerminal(&orig);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (signal(SIGINT, handle_signal) == SIG_ERR) {
        perror("Error registering signal handler");
        return 1;
    }
    tcgetattr(STDIN_FILENO, &orig);
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
    startCLI();
    // busScan();
    // dumpAddr(0x40);
    // drawWindow();
    // testLegs();

    return 0;
}
