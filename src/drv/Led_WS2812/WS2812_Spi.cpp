#include "WS2812_Spi.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

namespace {
constexpr uint32_t kSpiHz      = 2400000; // 3 SPI bits per WS2812 bit @ 1.25us -> 416.7ns/bit
constexpr uint8_t  kSpiMode    = SPI_MODE_0;
constexpr uint8_t  kSpiBits    = 8;
constexpr int      kResetBytes = 150;       // ~500us of low @2.4MHz, WS2812 needs >=50us to latch

// One WS2812 bit -> 3 SPI bits, MSB first.
constexpr uint8_t kSymbolZero = 0b100;
constexpr uint8_t kSymbolOne  = 0b110;

// Packs bits MSB-first into a growable byte buffer.
class BitPacker {
  public:
    explicit BitPacker(std::vector<uint8_t>& out) : _out(out) {}

    void pushBits(uint8_t value, int bitCount) {
        for (int i = bitCount - 1; i >= 0; --i) {
            if (_bitPos == 0) _out.push_back(0);
            if ((value >> i) & 0x1) _out.back() |= (0x80 >> _bitPos);
            _bitPos = (_bitPos + 1) % 8;
        }
    }

  private:
    std::vector<uint8_t>& _out;
    int                   _bitPos = 0;
};
} // namespace

Ws2812Spi::Ws2812Spi(int ledCount, const char* device) : _ledCount(ledCount) {
    _pixels.assign(ledCount * 3, 0);

    _fd = open(device, O_WRONLY);
    if (_fd < 0) return;

    uint8_t  mode = kSpiMode;
    uint8_t  bits = kSpiBits;
    uint32_t hz   = kSpiHz;
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
    _spiFrame.clear();
    _spiFrame.reserve(kResetBytes * 2 + _pixels.size() * 3 + 1);

    _spiFrame.insert(_spiFrame.end(), kResetBytes, 0x00); // reset / latch, low

    BitPacker packer(_spiFrame);
    for (uint8_t colorByte : _pixels)
        for (int bit = 7; bit >= 0; --bit)
            packer.pushBits((colorByte >> bit) & 0x1 ? kSymbolOne : kSymbolZero, 3);

    _spiFrame.insert(_spiFrame.end(), kResetBytes, 0x00); // reset / latch, low
}

bool Ws2812Spi::show() {
    if (!ok()) return false;
    _encode();
    return write(_fd, _spiFrame.data(), _spiFrame.size()) == (ssize_t)_spiFrame.size();
}
