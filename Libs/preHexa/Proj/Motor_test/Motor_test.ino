#include "h_brige_L298P.h"

#define CDCuart Serial
#define HWuart Serial1

L298P Rmotor(A0, A1, 5);
L298P Lmotor(A3, A2, 6);


void setup() {
  CDCuart.begin(9600);
  // while (!CDCuart) {}
  Lmotor.init();
  Rmotor.init();
  Lmotor.enable();
  Rmotor.enable();
  Lmotor.fwd();
  Rmotor.fwd();
  CDCuart.println("boot ok");
  delay(3000);
  Lmotor.stop();
  Rmotor.stop();
  delay(3000);
  Lmotor.brk();
  Rmotor.stop();

}

void loop() {

}