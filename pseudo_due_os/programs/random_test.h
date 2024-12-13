#include <VGA.h>

void randomtest(char** argv, int argc) {
  randomSeed(analogRead(A0));

  const char *names[] = {"bibarub", "binarin", "phonewave", "ivan vorvanin", "ivan otvalin", "vorvan divanin", "rostislav lastochkin", "rostislav emtrika", "baka emtrika"};
  size_t count = sizeof(names)/sizeof(*names);

  VGA.print(names[random(count)]);
  VGA.println(" is a khamlo");
}