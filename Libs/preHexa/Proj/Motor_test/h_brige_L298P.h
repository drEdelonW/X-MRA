#pragma once

class L298P {
public:
    L298P(int inA, int inB, int en):
        _inA(inA),
        _inB(inB),
        _en(en),
        _is_enable(false){};
    void init();
    void fwd();
    void rwd();
    void stop();
    void brk();
    void enable();
    void disable();

private:
    int _inA;
    int _inB;
    int _en;
    bool _is_enable;
};


