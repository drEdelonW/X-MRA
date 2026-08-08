#include "i2cBus_EndPiont.hpp"

i2cEndPoint::i2cEndPoint(i2cBus& bus, i2cAddr_t addr):
    _bus(bus),
    _addr(addr)
{}

bool i2cEndPoint::isInited() {
    return _bus.isInited();
}

bool i2cEndPoint::RegRead(uint8_t RegNum, uint8_p pByte) {
    return
        isInited() &&
        _bus._fSetAddr(_addr) &&
        _bus._fRegRead(RegNum, pByte);
}

bool i2cEndPoint::RegWrite(uint8_t RegNum, uint8_t pByte) {
    return
        isInited() &&
        _bus._fSetAddr(_addr) &&
        _bus._fRegWrite(RegNum, pByte);
}