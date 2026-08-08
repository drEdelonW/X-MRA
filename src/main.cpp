
#include "common_tools.h"   // ver_info
#include "signal_tools.hpp" // sysInit
#include "CLI.h"            // startCLI

#if 1
#include <csignal>

namespace {
    volatile sig_atomic_t g_running = 1;
    void on_sigint(int) { g_running = 0; }
}
#include "GPIO.hpp"
static int _main() {
    std::signal(SIGINT, on_sigint);

    GpioChip chip;
    if (!chip.ok())
        return 1;

    GpioLine gpio4(chip, 4);
    if (!gpio4.ok())
        return 1;

    printf("Toggling GPIO4. Ctrl+C to stop.\n");

#if 0
    gpiod_line_value value = GPIOD_LINE_VALUE_INACTIVE;
    while (g_running) {
        value = (value == GPIOD_LINE_VALUE_ACTIVE) ?
            GPIOD_LINE_VALUE_INACTIVE : GPIOD_LINE_VALUE_ACTIVE;

        gpio4.set(value);
        usleep(500 * 1000);
    }
#else
    bool value = true;
    while (g_running) {
        gpio4.setB(value = !value);
        usleep(500 * 1000);
    }
#endif

    return 0;
}
#endif

int main(int argc, char* argv[]) {
    ver_info(); // print git info
    sysInit();  // sys signal handle
    startCLI(); // main command processor
    // _main();

    return 0;
}
