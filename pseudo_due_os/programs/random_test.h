#include <VGA.h>    
    int wak_used = 0;
    randomSeed(analogRead(A0));
    const char *names[] = {"bibarub", "binarin", "phonewave", "ivan vorvanin", "ivan otvalin", "vorvan divanin", "rostislav lastochkin", "rostislav emtrika", "baka emtrika"};
    size_t count = sizeof(names)/sizeof(*names);
    // FIX repeat if wak isn't used
    if (wak_used == 1) {
    VGA.print(names[random(count)]);
    VGA.print(" is a khamlo\n");
    } else {
    randomSeed(analogRead(A0));
    const char *names[] = {"bibarub", "binarin", "phonewave", "ivan vorvanin", "ivan otvalin", "vorvan divanin", "rostislav lastochkin", "rostislav emtrika", "baka emtrika"};
    size_t count = sizeof(names)/sizeof(*names);
    wak_used = 1;
    VGA.print(names[random(count)]);
    VGA.print(" is a khamlo\n"); };