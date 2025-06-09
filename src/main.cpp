#include "common_tools.h"

#include "CLI.h"
#include "dpad.h"
// #include "robot_spec.hpp"

#include <csignal>
#include <cstdlib>

static inline void bodyInit() {
#ifdef PCA_Defined
    PWMarray[0].wakeUp();
    PWMarray[1].wakeUp();
    PWMarray[0].setFreq_Hz((Hertz)300);
    PWMarray[1].setFreq_Hz((Hertz)300);
#endif
}

static inline void bodyDeinit() {
#ifdef MAX_LEGS
    XMRA.DISARM();
#endif
#ifdef PCA_Defined
    PWMarray[0].sleepMode();
    PWMarray[1].sleepMode();
#endif
}


void handle_signal(int signal) {
    if ((signal == SIGINT) ||
        (signal == SIGTERM)) {
        printf("Received SIGINT (Ctrl+C). Exiting...\n"); fflush(stdout);
        // posix_socket.Close();
        bodyInit();
        restoreTerminal(&orig);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (signal(SIGINT, handle_signal) == SIG_ERR &&
        signal(SIGTERM, handle_signal) == SIG_ERR ) {
        perror("Error registering signal handler");
        return 1;
    }
    tcgetattr(STDIN_FILENO, &orig);
    ver_info();
#if 0
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

    bodyInit();

    startCLI();  // main command processor

    return 0;
}
