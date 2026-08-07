# Spider Shield for Raspberry Pi v1.3

## Pi-GPIO
             [VCC_3.3V] |  1 |  2 | [VCC_5V]
      (GPIO2) [I2C_SDA] |  3 |  4 | [VCC_5V]
      (GPIO3) [I2C_SCL] |  5 |  6 | [GND]
 Q4/R22 (GPIO4) [LOAD?] |  7 |  8 | [??] (GPIO14/UART_TX)
                  [GND] |  9 | 10 | [??] (GPIO15/UART_RX)
Q1/R2 (GPIO17) [BUZZER] | 11 | 12 | [WB_LED](GPIO10/SPI_MOSI)
        (GPIO27) [Trig] | 13 | 14 | [GND]
        (GPIO22) [Echo] | 15 | 16 | [??] (GPIO23)


Found device at address 0x4b [BNO080]
Found device at address 0x53 [ADXL345]