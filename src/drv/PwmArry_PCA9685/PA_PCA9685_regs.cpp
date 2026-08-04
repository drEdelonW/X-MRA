PCA_ChannelRegs PCA9685::_readChannelRegs(PwmChannel channel) {
    PCA_ChannelRegs regs;
    int rOffs = channel * 4;
    for (int i = 0; i < 4; ++i) {
        regs.raw[i] = _readRegister(LED0_ON_L + rOffs + i);
    }
    return regs;
}

void PCA9685::_writeChannelRegs(PwmChannel channel, const PCA_ChannelRegs& regs) {
    int rOffs = channel * 4;
    for (int i = 0; i < 4; ++i) {
        _iEP.RegWrite(LED0_ON_L + rOffs + i, regs.raw[i]);
    }
}