#pragma once

#define LED_NUM       (0x10) // Number of LED channels
enum PCA_Register_en:PCA_Register {
    MODE1         = 0x00, // Mode register 1
    MODE2         = 0x01, // Mode register 2

    LED0_ON_L     = 0x06, // LED0 ON low byte
    LED0_ON_H     = 0x07, // LED0 ON high byte
    LED0_OFF_L    = 0x08, // LED0 OFF low byte
    LED0_OFF_H    = 0x09, // LED0 OFF high byte

    ALL_LED_ON_L  = 0xFA, // All LEDs ON low byte
    ALL_LED_ON_H  = 0xFB, // All LEDs ON high byte
    ALL_LED_OFF_L = 0xFC, // All LEDs OFF low byte
    ALL_LED_OFF_H = 0xFD, // All LEDs OFF high byte

    PRE_SCALE     = 0xFE  // Prescale register for frequency
};