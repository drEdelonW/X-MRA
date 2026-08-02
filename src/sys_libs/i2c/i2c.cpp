#include "i2c.hpp"
#include <stdio.h>          // snprintf

#include <cerrno>           // errno
#include <cstring>          // strerror

#include <unistd.h>         // close write usleep
#include <fcntl.h>          // open
#include <sys/ioctl.h>      // ioctl
#include <linux/i2c-dev.h>  // I2C_SLAVE

i2cBus::i2cBus(uint8_t bus, bool initOnConstruct):
    _isInited(false),
    _bus(bus),
    _lastAddr(-1)
{
    snprintf(_fileName, sizeof(_fileName),
        "/dev/i2c-%d", _bus
    );
    if (initOnConstruct)
        Init();
}

bool i2cBus::Init() {
    _fd = open(_fileName, O_RDWR);
    if (_fd < 0) {
        fprintf(stderr,
            "[ERROR] %s: "
            "Failed to open I2C bus %d (%s). "
            "errno=%d\n",
            __func__,
            _bus, _fileName,
            errno
        );
        return false;
    }
    return _isInited = true;
}

void i2cBus::Deinit() {
    if (_fd != -1)
        close(_fd);
    _isInited = false;
}

bool i2cBus::_setAddres(i2cAddr_t addr) {
    if ((!_isInited) ||
        (addr <= 0)
    )   return false;

    _lastAddr = addr;

    if (ioctl(_fd, I2C_SLAVE, addr) < 0) {
        fprintf(stderr,
            "[ERROR] %s: "
            "Failed to set I2C address 0x%02X "
            "on bus %d (%s). "
            "errno=%d\n",
            __func__,
            addr,
            _bus, _fileName,
            errno
        );
        close(_fd); _fd = -1;
        return false;
    }

    return false;
}

