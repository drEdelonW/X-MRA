#include "WS2812_Spi.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define WS2812_SPI_HZ   (2400000) // 3 SPI bits per WS2812 bit @ 1.25us -> 416.7ns/bit
#define WS2812_SPI_MODE (SPI_MODE_0)
#define WS2812_SPI_BITS (8)

Ws2812Spi::Ws2812Spi(int ledCount, cStrRO device) :
    _ledCount(
        (ledCount < WS2812_MAX_LEDS) ?
            ledCount : WS2812_MAX_LEDS
    )
{
    if ((_fd = open(device, O_WRONLY)) < 0)
        return;

    uint8_t  mode = WS2812_SPI_MODE;
    uint8_t  bits = WS2812_SPI_BITS;
    uint32_t hz   = WS2812_SPI_HZ;
    if ((ioctl(_fd, SPI_IOC_WR_MODE, &mode)          < 0) ||
        (ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) ||
        (ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &hz)    < 0)
    ) {
        close(_fd);
        _fd = -1;
    }
}

Ws2812Spi::~Ws2812Spi() {
    if (_fd >= 0)
        close(_fd);
}

void Ws2812Spi::setPixel(int index, RGB color) {
    if ((index < 0) ||
        (index >= _ledCount)
        )   return;
    _pixels[index] = color;
}

void Ws2812Spi::fill(RGB color) {
    for (int i = 0; i < _ledCount; ++i)
        setPixel(i, color);
}
// WS2812 wire-level bytes: reset/latch terminator, and the two data-bit symbols, each re-encoded as 3 SPI bits, MSB first.
typedef enum : uint8_t {
    WS2812_TERMINATOR = 0b000, // reset/latch gap - byte held low
    WS2812_SYM_LO     = 0b100, // "0" bit -> 3 SPI bits (~417ns high / ~833ns low)
    WS2812_SYM_HI     = 0b110, // "1" bit -> 3 SPI bits (~833ns high / ~417ns low)
} Ws2812Symbol_t;
// Packs bitCount bits of value (MSB first) into buf, advancing _len/_bitPos.
void Ws2812Spi::_pushBits(uint8_t value) {
    for (int i = (BIT_MULT - 1); i >= 0; --i) {
        if (_bitPos == 0)
            _spiFrame[_len++] = WS2812_TERMINATOR;
        if ((value >> i) & 0x01)
            _spiFrame[_len - 1] |= (0x80 >> _bitPos);
        _bitPos = (_bitPos + 1) & 0x07;
    }
}
static inline Ws2812Symbol_t bit2WS(uint8_t in, int bit) {
    return
        ((in >> bit) & 0x01)?
            WS2812_SYM_HI :
            WS2812_SYM_LO;
}

bool Ws2812Spi::show() {
    if (!ok())
        return false;

    _len = 0;
    _bitPos = 0;
    for (int i = 0; i < _ledCount; ++i) {
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].r, bit));
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].g, bit));
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].b, bit));
    }
    // TODO: make cooldown by timestamp
    return
        (write(_fd, _spiFrame, _len) == (ssize_t)_len);
}
