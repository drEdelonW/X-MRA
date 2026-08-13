#pragma once

#include "types.h"

// Minimal SPI-clocked driver for WS2812 / WS2812B addressable LEDs.
//
// WS2812 needs ~1.25us bit slots with tight (+-150ns) high/low timing that a
// userspace GPIO toggle (even through libgpiod) cannot hit reliably once the
// kernel scheduler gets involved. Instead this drives the strip through the
// Pi's SPI0 *hardware* shift clock: each WS2812 data bit is re-encoded as
// 3 SPI bits, so the timing comes from the SPI peripheral's clock divider,
// not from CPU scheduling.
//
//   WS2812 "0" bit -> SPI 0b100   (~417ns high / ~833ns low)
//   WS2812 "1" bit -> SPI 0b110   (~833ns high / ~417ns low)
//   SPI clock = 2.4MHz -> 416.7ns/bit -> 3 bits = 1.25us WS2812 bit slot
//
// Requires /dev/spidev0.0 (raspi-config -> Interface Options -> SPI, or
// `dtparam=spi=on` in /boot/firmware/config.txt + reboot) and GPIO10
// (SPI0 MOSI) wired to the strip's DIN - the "WB_LED" pad on the Spider
// Shield (see Docs/FreenoveChassis/FreeNove.MD).

#include "WS_RGB.hpp"

#define WS2812_MAX_LEDS    (32)  // static buffer cap - raise if a longer strip ever gets wired
// #define WS2812_RESET_BYTES (150) // ~500us of low @2.4MHz, WS2812 needs >=50us to latch
#define BIT_MULT  (3)
// #define WS2812_FRAME_BYTES ((WS2812_MAX_LEDS * 3) * BIT_MULT) // payload
class Ws2812Spi {
  public:
    explicit Ws2812Spi(int ledCount, cStrRO device = "/dev/spidev0.0");
    ~Ws2812Spi();

    Ws2812Spi(const Ws2812Spi&) = delete;
    Ws2812Spi& operator=(const Ws2812Spi&) = delete;

    bool ok() const { return _fd >= 0; }
    int  getNumLeds() const { return _ledCount; }

    void setPixel(int index, RGB color);
    void fill(RGB color);
    void clear() { fill(RGB{}); }

    bool show(); // encodes _pixels[] and pushes it out over SPI

  private:
    int     _ledCount;
    int     _fd = -1;
    RGB     _pixels[WS2812_MAX_LEDS] = {};
    uint8_t _spiFrame[sizeof(_pixels) * BIT_MULT] = {}; // payload

    int     _len;
    int     _bitPos;
    void    _pushBits(uint8_t value);
};
