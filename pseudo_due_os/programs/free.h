extern "C" char* sbrk(int incr);

void freeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

VGA.print(F("- SRAM left: "));
VGA.println(freeRam());