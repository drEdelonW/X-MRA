#include "ADS7830.hpp"

uint8_t ADS7830::readChannel(uint8_t channel) {
    /* ADS7830 mux select is non-sequential (datasheet Table 1):
       select code 0..7 -> physical channel 0,4,1,5,2,6,3,7. */
    static int _remap[8] = {0, 4, 1, 5, 2, 6, 3, 7};
    uint8_t cmd = 0x84 | (_remap[channel] << 4); // SD=1 single-ended, PD1:PD0=11 ref+ADC on

    uint8_t value = 0;
    _iEP.RegRead(cmd, &value);
    return value;
}

void ADS7830::readAll(uint8_t out[8]) {
    for (int ch = 0; ch < 8; ++ch)
        out[ch] = readChannel(ch);
}
