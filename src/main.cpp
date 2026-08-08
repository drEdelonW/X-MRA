#include "common_tools.h"   // ver_info
#include "signal_tools.hpp" // sysInit
#include "CLI.h"            // startCLI

#if 1
#include <csignal>
volatile sig_atomic_t g_running = 1;
void on_sigint(int) { g_running = 0; }

#include "GPIO.hpp"
static void _gpio_test() {
    std::signal(SIGINT, on_sigint);

    GpioChip chip;
    if (!chip.ok())
        return;

    GpioLine gpio4(chip, 4);
    if (!gpio4.ok())
        return;

    printf("Toggling GPIO4. Ctrl+C to stop.\n");

    bool value = true;
    while (g_running) {
        gpio4.setB(value = !value);
        usleep(500 * 1000);
    }
    return;
}
#endif



int main(int argc, char* argv[]) {
    ver_info(); // print git info
    sysInit();  // sys signal handle
    startCLI(); // main command processor
    // _gpio_test();

    return 0;
}
