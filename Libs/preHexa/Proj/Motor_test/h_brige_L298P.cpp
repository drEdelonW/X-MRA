#include "h_brige_L298P.h"
#include <Arduino.h>

void L298P::init(){
    pinMode(_inA, OUTPUT);    digitalWrite(_inA, 0);
    pinMode(_inB, OUTPUT);    digitalWrite(_inB, 0);
    pinMode(_en,  OUTPUT);    digitalWrite(_en,  0);
}

void L298P::fwd(){
    digitalWrite(_inA, 1);
    digitalWrite(_inB, 0);
}

void L298P::rwd(){
    digitalWrite(_inA, 0);
    digitalWrite(_inB, 1);
}

void L298P::stop(){
    digitalWrite(_inA, 0);
    digitalWrite(_inB, 0);
}

void L298P::brk(){
    digitalWrite(_inA, 1);
    digitalWrite(_inB, 1);
}

void L298P::enable(){
    digitalWrite(_en, 1);
}

void L298P::disable(){
    digitalWrite(_en, 0);
}

