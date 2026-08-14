#include "WS2812_cWrap.h"
#include "WS2812_Spi.hpp"
#include "terminal_tools.h"

#include <unistd.h> // usleep()

#define WS_LED_COUNT    (7) // TODO: adjust to the actual number of LEDs wired on WB_LED (GPIO10)

void wsLedTest() {
    Ws2812Spi strip(WS_LED_COUNT);
    if (!strip.ok()) {
        ERROR("WS2812: failed to open /dev/spidev0.0 - enable SPI first "
              "(raspi-config -> Interface Options -> SPI, or uncomment "
              "dtparam=spi=on in /boot/firmware/config.txt, then reboot)");
        return;
    }

    float Bright = 0.01f;
    LOG("WS2812: red\n");   strip.fill(Bright * Colors::Red);   strip.show(); usleep(600000);
    LOG("WS2812: green\n"); strip.fill(Bright * Colors::Green); strip.show(); usleep(600000);
    LOG("WS2812: blue\n");  strip.fill(Bright * Colors::Blue);  strip.show(); usleep(600000);

    LOG("WS2812: chase (confirms per-pixel addressing / real strip length)\n");
    for (int pass = 0; pass < 2; ++pass) {
        for (int i = 0; i < strip.getNumLeds(); i++) {
            strip.clear(); strip.setPixel(i, Bright * Colors::Blue); strip.show(); usleep(240000);
        }
    }

    LOG("WS2812: off\n");
    strip.clear();
    strip.show();
}
