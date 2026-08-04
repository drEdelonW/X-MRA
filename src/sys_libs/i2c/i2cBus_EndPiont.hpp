#pragma once
#include "types.h"

typedef enum:int8_t {   // negative not allowed - upper bit is data direction status
    I2C_INVALID_ADDR  = -1,   // sentinel: no device / lookup failed, not a wire value
    I2C_DEV_UNKNOWN   = 0x00,

    /* Reserved: 0x00-0x07 - general call, CBUS, HS-mode master codes */
    I2C_RESERVED_GENERAL_CALL      = 0x00, // addr 0x00 + W: General Call - broadcasts to all devices that ack it; addr 0x00 + R is the separate START byte procedure (same 7-bit addr, different R/W bit)
    I2C_RESERVED_CBUS              = 0x01, // reserved for compatibility with old CBUS-format devices, do not assign
    I2C_RESERVED_DIFFERENT_BUS_FMT = 0x02, // reserved for devices using a different bus format than standard I2C
    I2C_RESERVED_FUTURE_LOW_1      = 0x03, // reserved for future use by I2C-bus committee
    I2C_RESERVED_HS_MASTER_CODE_1  = 0x04, // High-Speed mode master code (0000 1xx block, 0x04-0x07) - sent before HS-mode transfer to signal bus masters, never a slave address
    I2C_RESERVED_HS_MASTER_CODE_2  = 0x05,
    I2C_RESERVED_HS_MASTER_CODE_3  = 0x06,
    I2C_RESERVED_HS_MASTER_CODE_4  = 0x07,


    /* Reserved: 0x78-0x7F - 10-bit addressing prefix and future use */
    I2C_RESERVED_10BIT_ADDR_1  = 0x78, // 10-bit addressing first byte (1111 0xx block, 0x78-0x7B) - the two low bits carry the MSBs of the 10-bit target address, second byte carries the remaining 8 bits
    I2C_RESERVED_10BIT_ADDR_2  = 0x79,
    I2C_RESERVED_10BIT_ADDR_3  = 0x7A,
    I2C_RESERVED_10BIT_ADDR_4  = 0x7B,
    I2C_RESERVED_FUTURE_HIGH_1 = 0x7C, // reserved for future purposes (1111 1xx block, 0x7C-0x7F), not part of 10-bit addressing
    I2C_RESERVED_FUTURE_HIGH_2 = 0x7D,
    I2C_RESERVED_FUTURE_HIGH_3 = 0x7E,
    I2C_RESERVED_FUTURE_HIGH_4 = 0x7F,
} i2cAddr_t;

class i2cEndPoint; // fwd decl
class i2cBus {
  public:
    i2cBus(uint8_t bus = 0, bool initOnConstruct = false);
    ~i2cBus();
    bool Init();  // openBus()
    void Deinit();// closeBus()
    bool isInited() { return _isInited; }

    bool ProbeDevice(i2cAddr_t adr);

  private:
    friend class i2cEndPoint;          // only endpoints get to switch addr / do raw IO

    bool setAddr(i2cAddr_t adr);
    bool RegRead(uint8_t RegNum, uint8_p pByte);
    bool RegWrite(uint8_t RegNum, uint8_t pByte);

    bool      _isInited;
    uint8_t   _bus;
    i2cAddr_t _lastAddr;
    int       _fd;
    char      _fileName[20];

    bool _setAddres(i2cAddr_t addr);
    bool _Read(uint8_p pByte);
    bool _Write(uint8_t pByte);
    bool _setRegNum(uint8_t RegNum);
};

class i2cEndPoint {
  public:
    i2cEndPoint(i2cBus& bus, i2cAddr_t addr);

    bool isInited();
    bool RegRead(uint8_t RegNum, uint8_p pByte);
    bool RegWrite(uint8_t RegNum, uint8_t pByte);

    i2cAddr_t addr() const { return _addr; }

  private:
    i2cBus&   _bus;
    i2cAddr_t _addr;
};
