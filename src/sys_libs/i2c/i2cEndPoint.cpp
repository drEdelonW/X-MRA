#include "i2cEndPoint.hpp"

i2cEndPoint::i2cEndPoint(i2cBus& bus, i2cAddr_t addr):
    _bus(bus),
    _addr(addr)
{}

bool i2cEndPoint::isInited() {
    return _bus.isInited();
}

bool i2cEndPoint::RegRead(uint8_t RegNum, uint8_p pByte) {
    return
        _bus.setAddr(_addr) &&
        _bus.RegRead(RegNum, pByte);
}

bool i2cEndPoint::RegWrite(uint8_t RegNum, uint8_t pByte) {
    return
        _bus.setAddr(_addr) &&
        _bus.RegWrite(RegNum, pByte);
}