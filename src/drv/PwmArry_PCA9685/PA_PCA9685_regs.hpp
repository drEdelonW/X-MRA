#pragma once
#include <cstdint>

union PCA_Mode1Reg {
    struct __attribute__((packed)) {
        uint8_t allCall : 1; // bit0: respond to LED All Call address
        uint8_t sub3    : 1; // bit1: respond to I2C-bus subaddress 3
        uint8_t sub2    : 1; // bit2: respond to I2C-bus subaddress 2
        uint8_t sub1    : 1; // bit3: respond to I2C-bus subaddress 1
        uint8_t sleep   : 1; // bit4: low power mode, oscillator off
        uint8_t ai      : 1; // bit5: register auto-increment enable
        uint8_t extClk  : 1; // bit6: use EXTCLK pin clock
        uint8_t restart : 1; // bit7: restart enabled
    } field;
    uint8_t raw;
};

union PCA_Mode2Reg {
    struct __attribute__((packed)) {
        uint8_t outne  : 2; // bits0-1: output driver state when OE=1
        uint8_t outdrv : 1; // bit2: totem pole (1) / open-drain (0)
        uint8_t och    : 1; // bit3: outputs change on ACK (1) / STOP (0)
        uint8_t invrt  : 1; // bit4: output logic state invert
        uint8_t        : 3; // bits5-7: reserved
    } field;
    uint8_t raw;
};

// One channel = 4 consecutive registers: LEDn_ON_L/H, LEDn_OFF_L/H.
// Bitfield layout matches the datasheet on little-endian targets only
// (GCC/Clang pack LSB-first there). Verified by static_assert below.
union PCA_ChannelRegs {
    struct __attribute__((packed)) {
        uint8_t on_l;
        uint8_t on_h;
        uint8_t off_l;
        uint8_t off_h;
    } bytes;

    struct __attribute__((packed)) {
        uint16_t onCount  : 12; // 0..4095, ON edge position
        uint16_t onFull   : 1;  // LEDn_FULL_ON  bit
        uint16_t          : 3;  // reserved
        uint16_t offCount : 12; // 0..4095, OFF edge position
        uint16_t offFull  : 1;  // LEDn_FULL_OFF bit
        uint16_t          : 3;  // reserved
    } field;

    uint8_t raw[4];
};

static_assert(sizeof(PCA_ChannelRegs) == 4, "PCA_ChannelRegs must map 1:1 onto the 4 device registers");

#include "PWMChannel.hpp"   // PwmChNum
struct __attribute__((packed)) PCA9685_RegMap {
    PCA_Mode1Reg    mode1;                  // 0x00
    PCA_Mode2Reg    mode2;                  // 0x01
    uint8_t         subAddr[3];             // 0x02-0x04
    uint8_t         allCallAddr;            // 0x05
    PCA_ChannelRegs channel[PwmChNum];      // 0x06-0x45
    uint8_t         _reserved[0xFA - 0x46]; // 0x46-0xF9, unused on silicon
    PCA_ChannelRegs allLed;                 // 0xFA-0xFD
    uint8_t         preScale;               // 0xFE
    uint8_t         testMode;               // 0xFF, factory-only, never write
};
