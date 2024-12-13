#include <VGA.h>

void argtest(char** argv, int argc) {
  VGA.print("Your arguments:\n");
  for (int i = 0; i < argc; ++i) {
    VGA.print(i);
    VGA.print(": ");
    VGA.print(argv[i]);
    VGA.print('\n');
  }
}