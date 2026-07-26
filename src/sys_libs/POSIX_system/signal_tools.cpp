#include "signal_tools.hpp"
#include <cstdlib> // exit()
#include <csignal>

#include "common_tools.h"

static void handle_signal(int signal) {
    if ((signal == SIGINT) ||
        (signal == SIGTERM) ||
        (signal == SIGHUP)     // TODO: fix messages
        ) {
        WARNING("Received SIGINT (Ctrl+C). Exiting...\n");
        sysDeinit();
        exit(0);
    }
    else {
        WARNING("Unhandled signal is[%d]\n", signal);
    }
}

#include "dpad.h" // orig
void sysInit() {
    // see [/usr/include/aarch64-linux-gnu/bits/signum-generic.h]
    if ((signal(SIGINT, handle_signal) == SIG_ERR) &&
        (signal(SIGTERM, handle_signal) == SIG_ERR) &&
        (signal(SIGHUP, handle_signal) == SIG_ERR)
        ) {
        perror("Error registering signal handler");
        exit(1);
    }

    tcgetattr(STDIN_FILENO, &orig);

#if 0 //def PCA_Defined       // TODO: test - going to be called from constructor
    PWMarray[0].wakeUp(); PWMarray[0].setFreq_Hz((Hertz)300);
    PWMarray[1].wakeUp(); PWMarray[1].setFreq_Hz((Hertz)300);
#endif
}

void sysDeinit() {
#if 0 //def MAX_LEGS       // TODO: test - going to be called from destructor
    XMRA.DISARM();
#endif
#if 0 //def PCA_Defined       // TODO: test - going to be called from destructor
    PWMarray[0].sleepMode();
    PWMarray[1].sleepMode();
#endif
    restoreTerminal(&orig);
}