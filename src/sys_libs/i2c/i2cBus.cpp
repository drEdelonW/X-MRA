#include "i2cBus_EndPiont.hpp"
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
    _lastAddr(I2C_INVALID_ADDR)
{
    snprintf(_fileName, sizeof(_fileName),
        "/dev/i2c-%d", _bus
    );
    if (initOnConstruct)
        Init();
}

i2cBus::~i2cBus() {
    Deinit();
}
bool i2cBus::Init() {
    _isInited = (_fd = open(_fileName, O_RDWR)) >= 0;
    if (!_isInited) {
        fprintf(stderr,
            "[ERROR] %s: "
            "Failed to open I2C bus %d (%s). "
            "errno=%d\n",
            __func__,
            _bus, _fileName,
            errno
        );
    } else {
        SoftResetAllDevices();
    }
    return _isInited;
}

void i2cBus::Deinit() {
    if (_fd != -1) {
        close(_fd);
        _fd = -1;
    }
    _isInited = false;
}

bool i2cBus::_setAddres(i2cAddr_t addr) {
    return
        _isInited &&
        (ioctl(_fd, I2C_SLAVE, addr) >= 0);
}

bool i2cBus::_Read(uint8_p pByte) {  return ( read(_fd,  pByte, 1) == 1); }
bool i2cBus::_Write(uint8_t pByte) { return (write(_fd, &pByte, 1) == 1); }

bool i2cBus::_setRegNum(uint8_t RegNum) {
    bool ret = _Write(RegNum);
#if 0
    if (!ret)
        printf("Failed to write register address 0x%x\n", RegNum);
#endif
    return ret;
}

bool i2cBus::_fSetAddr(i2cAddr_t adr) {
    if (adr < 0)
        return false;
    if (_lastAddr == adr)
        return true;

    // TODO: make here Thread safety
    _lastAddr = adr;
    bool ret = _setAddres(adr);
#if 0
    if (!ret) {
        fprintf(stderr,
            "[ERROR] %s: "
            "Failed to set I2C address 0x%02X "
            "on bus %d (%s). "
            "errno=%d\n",
            __func__,
            adr,
            _bus, _fileName,
            errno
        );
    }
#endif
    return ret;
}


bool i2cBus::SoftResetAllDevices() {
    return
        _setAddres(I2C_RESERVED_GENERAL_CALL) &&
        _Write(I2C_GC_RESET_AND_PROGRAM); // SWRST command byte, per NXP general-call spec
}

bool i2cBus::ProbeDevice(i2cAddr_t adr) {
    uint8_t buf;
    return
        _fSetAddr(adr) &&
        _Read(&buf);
}

bool i2cBus::_fRegRead(uint8_t RegNum, uint8_p pByte) {
    return
        _setRegNum(RegNum) &&
        _Read(pByte);
}
bool i2cBus::_fRegWrite(uint8_t RegNum, uint8_t pByte) {
    uint8_t buf[2] = {RegNum, pByte};
    return write(_fd, buf, sizeof(buf)) == sizeof(buf);  // must be one I2C transaction: separate write()s would send reg and value as two unrelated START/STOP packets
}
