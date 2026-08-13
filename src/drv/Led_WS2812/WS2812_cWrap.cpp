#include "WS2812_cWrap.h"
#include "WS2812_Spi.hpp"
#include "terminal_tools.h"

#include <unistd.h> // usleep()

#define WS_LED_COUNT   8  // TODO: adjust to the actual number of LEDs wired on WB_LED (GPIO10)
#define WS_TEST_BRIGHT 32 // keep low for a first bring-up test (0-255)

void wsLedTest() {
    Ws2812Spi strip(WS_LED_COUNT);
    if (!strip.ok()) {
        ERROR("WS2812: failed to open /dev/spidev0.0 - enable SPI first "
              "(raspi-config -> Interface Options -> SPI, or uncomment "
              "dtparam=spi=on in /boot/firmware/config.txt, then reboot)");
        return;
    }

    LOG("WS2812: red\n");
    strip.fill(WS_TEST_BRIGHT, 0, 0); strip.show(); usleep(600000);

    LOG("WS2812: green\n");
    strip.fill(0, WS_TEST_BRIGHT, 0); strip.show(); usleep(600000);

    LOG("WS2812: blue\n");
    strip.fill(0, 0, WS_TEST_BRIGHT); strip.show(); usleep(600000);

    LOG("WS2812: chase (confirms per-pixel addressing / real strip length)\n");
    for (int pass = 0; pass < 2; ++pass) {
        for (int i = 0; i < WS_LED_COUNT; ++i) {
            strip.clear();
            strip.setPixel(i, 0, 0, WS_TEST_BRIGHT);
            strip.show();
            usleep(120000);
        }
    }

    LOG("WS2812: off\n");
    strip.clear();
    strip.show();
}
