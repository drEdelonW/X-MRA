#pragma once

#include <cstdint>
#include <vector>

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
class Ws2812Spi {
  public:
    explicit Ws2812Spi(int ledCount, const char* device = "/dev/spidev0.0");
    ~Ws2812Spi();

    Ws2812Spi(const Ws2812Spi&) = delete;
    Ws2812Spi& operator=(const Ws2812Spi&) = delete;

    bool ok() const { return _fd >= 0; }

    void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);
    void fill(uint8_t r, uint8_t g, uint8_t b);
    void clear() { fill(0, 0, 0); }

    bool show(); // encodes _pixels[] and pushes it out over SPI

  private:
    int                  _fd = -1;
    int                  _ledCount;
    std::vector<uint8_t> _pixels;   // wire order G,R,B - 3 bytes per LED
    std::vector<uint8_t> _spiFrame; // reset + 3-bits-per-WS2812-bit payload + reset

    void _encode();
};
