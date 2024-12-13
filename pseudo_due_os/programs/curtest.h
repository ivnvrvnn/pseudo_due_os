#include <VGA.h>

void curtest(char** argv, int argc) {
  VGA.print("X: ");
  VGA.print(VGA.tx);
  VGA.print(" Y: ");
  VGA.print(VGA.ty);
  VGA.println();
}