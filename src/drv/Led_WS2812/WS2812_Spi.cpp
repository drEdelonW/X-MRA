#include "WS2812_Spi.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


Ws2812Spi::Ws2812Spi(int ledCount, cStrRO device) :
    _ledCount(
        (ledCount < WS2812_MAX_LEDS) ?
            ledCount : WS2812_MAX_LEDS
    )
{
    if ((_fd = open(device, O_WRONLY)) < 0)
        return;

    uint8_t  _mode = SPI_MODE_0;
    uint8_t  _bits = 8;
    uint32_t _hz   = 2400000; // 3 SPI bits per WS2812 bit @ 1.25us -> 416.7ns/bit
    if ((ioctl(_fd, SPI_IOC_WR_MODE, &_mode)          < 0) ||
        (ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &_bits) < 0) ||
        (ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_hz)    < 0)
    ) {
        close(_fd);
        _fd = -1;
    }
}

Ws2812Spi::~Ws2812Spi() {
    if (_fd >= 0)
        close(_fd);
}

void Ws2812Spi::setPixel(unsigned int index, RGB color) {
    if (index >= _ledCount)
        return;
    _pixels[index] = color;
}

void Ws2812Spi::fill(RGB color) {
    for (int i = 0; i < _ledCount; ++i)
        setPixel(i, color);
}
// One WS2812 data bit, re-encoded as 3 SPI bits, MSB first.
typedef enum : uint8_t {
    WS2812_ZERO_BYTE = 0b000, // fresh-byte start value, not a WS2812 bit
    WS2812_SYM_LO    = 0b100, // "0" bit -> 3 SPI bits (~417ns high / ~833ns low)
    WS2812_SYM_HI    = 0b110, // "1" bit -> 3 SPI bits (~833ns high / ~417ns low)
} Ws2812Symbol_t;

// Packs value's low BIT_MULT bits (MSB first) into _spiFrame, advancing _len/_bitPos.
void Ws2812Spi::_pushBits(uint8_t value) {
    for (int i = (BIT_MULT - 1); i >= 0; --i) {
        if (_bitPos == 0)
            _spiFrame[_len++] = WS2812_ZERO_BYTE;
        if ((value >> i) & 0x01)
            _spiFrame[_len - 1] |= (0x80 >> _bitPos);
        (++_bitPos) &= 0x07;
    }
}

// One pixel color bit -> its WS2812 SPI symbol.
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
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].g, bit));
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].r, bit));
        for (int bit = 7; bit >= 0; --bit)  _pushBits(bit2WS(_pixels[i].b, bit));
    }
    // TODO: make cooldown by timestamp
    return
        (write(_fd, _spiFrame, _len) == (ssize_t)_len);
}
