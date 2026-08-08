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

// Second byte sent after the General Call address (0x00, W).
// Per NXP UM10204 I2C-bus specification, "General Call" section.
typedef enum : uint8_t {
    I2C_GC_INVALID              = 0x00, // not a legal second byte - would duplicate the general call address itself, must never be sent
    I2C_GC_CBUS_ADDRESS         = 0x01, // reserved: marks following data as targeted at a legacy CBUS-compatible device, not a real I2C part; true I2C devices ignore this and the data that follows
    I2C_GC_RESERVED_1           = 0x02, // reserved for future use by the I2C-bus committee
    I2C_GC_RESERVED_2           = 0x03, // reserved for future use by the I2C-bus committee
    I2C_GC_PROGRAM_ADDRESS      = 0x04, // devices with a hardware-latched programmable address load a new address; no state reset
    I2C_GC_RESERVED_3           = 0x05, // reserved for future use by the I2C-bus committee
    I2C_GC_RESET_AND_PROGRAM    = 0x06, // full reset to power-up defaults, then (if the device supports it) latch a new programmable address
                                        // this is the SWRST byte PCA9685 (and most simple I2C peripherals) actually act on
    I2C_GC_RESERVED_4           = 0x07, // reserved for future use by the I2C-bus committee

    // 0x08-0xFE with bit0==0: "Hardware General Call" - a bus master with no
    // fixed address of its own broadcasts ITS OWN 7-bit address in bits[7:1]
    // to request service from another master; bit0=0 signals it will act as
    // a slave on the immediately following transfer. Not a fixed enum value -
    // decode with: (secondByte >= 0x08) && ((secondByte & 0x01) == 0)
} i2cGeneralCallCmd_t;


class i2cEndPoint; // fwd decl
class i2cBus {
  public:
    i2cBus(uint8_t bus = 0, bool initOnConstruct = false);
    ~i2cBus();
    bool Init();
    void Deinit();
    bool isInited() const { return _isInited; }

    bool SoftResetAllDevices();
    bool ProbeDevice(i2cAddr_t adr);

  private:
    friend class i2cEndPoint;   // only endpoints get to switch addr / do raw IO
    bool _fSetAddr(i2cAddr_t adr);
    bool _fRegRead(uint8_t RegNum, uint8_p pByte);
    bool _fRegWrite(uint8_t RegNum, uint8_t pByte);

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
