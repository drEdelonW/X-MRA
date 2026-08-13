#include "WS2812_Spi.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define WS2812_SPI_HZ   (2400000) // 3 SPI bits per WS2812 bit @ 1.25us -> 416.7ns/bit
#define WS2812_SPI_MODE (SPI_MODE_0)
#define WS2812_SPI_BITS (8)

// WS2812 wire-level bytes: reset/latch terminator, and the two data-bit
// symbols, each re-encoded as 3 SPI bits, MSB first.
typedef enum : uint8_t {
    WS2812_TERMINATOR = 0x00,  // reset/latch gap - byte held low
    WS2812_SYM_LO     = 0b100, // "0" bit -> 3 SPI bits (~417ns high / ~833ns low)
    WS2812_SYM_HI     = 0b110, // "1" bit -> 3 SPI bits (~833ns high / ~417ns low)
} Ws2812Symbol_t;

// Packs bitCount bits of value (MSB first) into buf, advancing *len/*bitPos.
static void _pushBits(uint8_p buf, int* len, int* bitPos, uint8_t value, int bitCount) {
    for (int i = bitCount - 1; i >= 0; --i) {
        if (*bitPos == 0) buf[(*len)++] = 0;
        if ((value >> i) & 0x1) buf[*len - 1] |= (0x80 >> *bitPos);
        *bitPos = (*bitPos + 1) % 8;
    }
}

Ws2812Spi::Ws2812Spi(int ledCount, cStrRO device) :
    _ledCount(ledCount > WS2812_MAX_LEDS ? WS2812_MAX_LEDS : ledCount)
{
    _fd = open(device, O_WRONLY);
    if (_fd < 0) return;

    uint8_t  mode = WS2812_SPI_MODE;
    uint8_t  bits = WS2812_SPI_BITS;
    uint32_t hz   = WS2812_SPI_HZ;
    if (ioctl(_fd, SPI_IOC_WR_MODE, &mode)          < 0 ||
        ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 ||
        ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &hz)    < 0
    ) {
        close(_fd);
        _fd = -1;
    }
}

Ws2812Spi::~Ws2812Spi() {
    if (_fd >= 0) close(_fd);
}

void Ws2812Spi::setPixel(int index, RGB color) {
    if (index < 0 || index >= _ledCount) return;
    _pixels[index * 3 + 0] = color.g; // WS2812 wire order is G,R,B
    _pixels[index * 3 + 1] = color.r;
    _pixels[index * 3 + 2] = color.b;
}

void Ws2812Spi::fill(RGB color) {
    for (int i = 0; i < _ledCount; ++i) setPixel(i, color);
}

void Ws2812Spi::_encode() {
    int len = 0;
    for (int i = 0; i < WS2812_RESET_BYTES; ++i)
        _spiFrame[len++] = WS2812_TERMINATOR; // reset / latch, low

    int bitPos = 0;
    for (int i = 0; i < _ledCount * 3; ++i)
        for (int bit = 7; bit >= 0; --bit)
            _pushBits(_spiFrame, &len, &bitPos,
                ((_pixels[i] >> bit) & 0x1) ? WS2812_SYM_HI : WS2812_SYM_LO, 3);

    for (int i = 0; i < WS2812_RESET_BYTES; ++i)
        _spiFrame[len++] = WS2812_TERMINATOR; // reset / latch, low

    _frameLen = len;
}

bool Ws2812Spi::show() {
    if (!ok()) return false;
    _encode();
    return write(_fd, _spiFrame, _frameLen) == (ssize_t)_frameLen;
}
