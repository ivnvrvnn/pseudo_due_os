#include <VGA.h>

void circle(char** argv, int argc) {
  if (argc != 2) {
    VGA.println("Usage: .circle <radius>");
    return;
  }

  int radius = atoi(argv[1]);
  if (radius <= 0) {
    VGA.println("Invalid radius");
    return;
  }

  if (radius > 255) {
    VGA.println("Radius too large");
    return;
  }

  
  VGA.print("Drawing a circle with radius: ");
  VGA.println(radius);
  VGA.drawCircle(32, 32, radius, 1);
  VGA.fillCircle(32, 32, radius, 1);
}