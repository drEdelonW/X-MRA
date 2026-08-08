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
    else
        WARNING("Unhandled signal is[%d]\n", signal);
}

#include <termios.h>
struct termios Orig;

void sysInit() {
    // see [/usr/include/aarch64-linux-gnu/bits/signum-generic.h]
    if ((signal(SIGINT, handle_signal) == SIG_ERR) &&
        (signal(SIGTERM, handle_signal) == SIG_ERR) &&
        (signal(SIGHUP, handle_signal) == SIG_ERR)
        ) {
        perror("Error registering signal handler");
        exit(1);
    }

    tcgetattr(STDIN_FILENO, &Orig);
}


extern "C" { void restoreTerminal(const struct termios* orig); }
void sysDeinit() {
#if 0 //def MAX_LEGS       // TODO: test - going to be called from destructor
    XMRA.DISARM();
#endif
    restoreTerminal(&Orig);
}