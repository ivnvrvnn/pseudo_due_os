// pseudo os for due

// including required libraries
#include <KeyboardController.h>
#include <VGA.h>
//#include <string.h>

#include "config.h"

#define MAX_ARGUMENTS 8

#define OEM_ENTER 40
#define OEM_BACKSPACE 42

#ifdef LOG_OUTPUT_TO_SERIAL
#define LOGSERIAL(...) do { Serial.print(__VA_ARGS__); } while (0)
#else
#define LOGSERIAL(...) do {} while (0)
#endif

USBHost usb;
KeyboardController keyboard(usb);

// keybuffer
#ifdef LARGE_KEYBUFFER
unsigned keypointer;

#ifndef KEYBUFFER_SIZE
#define KEYBUFFER_SIZE 1024
#endif

#else /* LARGE_KEYBUFFER */

uint8_t keypointer;

#ifndef KEYBUFFER_SIZE
#define KEYBUFFER_SIZE 256
#endif

#endif /* LARGE_KEYBUFFER */
char keybuffer[KEYBUFFER_SIZE];

typedef struct Command {
  const char* name;
  void (*handler)(char** argv, int argc);
} Command;

void reset(char**, int) {
  NVIC_SystemReset();
}

void circle(char**, int);
void argtest(char**, int);
void test(char**, int);
void newtondemo(char**, int);
void randomtest(char**, int);
void curtest(char**, int);
#include "programs/circle.h"
#include "programs/argtest.h"
#include "programs/test.h"
#include "programs/newton_demo.h"
#include "programs/random_test.h"
#include "programs/curtest.h"

const Command commands[] = {
  {"circle", &circle},
  {"test", &test},
  {"reset", &reset},
  {"newton_demo", &newtondemo},
  {"curtest", &curtest},
  {"randomtest", &randomtest},
  {"argtest", &argtest}
};
size_t numCommands = sizeof(commands) / sizeof(Command);

void processArguments(char** argv, int argc) {
  if (argc < 1) {
    return;
  }
  for (size_t i = 0; i < numCommands; ++i) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      commands[i].handler(argv, argc);
      return;
    }
  }
  VGA.print("Command not found: ");
  VGA.println(argv[0]);
}

void commandProcessor() {
  if (keybuffer[0] != '.') {
    return;
  }
  char *argv[MAX_ARGUMENTS];
  int argc = 1;
  char* token = strtok(keybuffer+1, " ");
  if (token == NULL) {
    return;
  }
  argv[0] = token;
  while (token = strtok(NULL, " ")) {
    if (argc == MAX_ARGUMENTS) {
      LOGSERIAL("too many arguments");
      VGA.print("too many arguments.\n");
      return;
    }
    argv[argc++] = token;
  }
  processArguments(argv, argc);
  VGA.print('\n');
}

void keyPressed() {
  char key = keyboard.getKey();
  uint8_t oemCode = keyboard.getOemKey();

  LOGSERIAL("Pressed: ");
  LOGSERIAL(key);
  LOGSERIAL('\n');

  printKey(key, oemCode, keyboard.getModifiers());

  if (oemCode == OEM_ENTER) {
    VGA.print('\n');

    LOGSERIAL("keybuffer: ");
    LOGSERIAL(keybuffer);
    LOGSERIAL('\n');

    commandProcessor();
    if (keypointer != 0) {
      LOGSERIAL("resetting the buffer\n");

      keypointer = 0;
      keybuffer[0] = '\0';
    }
    VGA.print("> ");
    return;
  }
  if (oemCode == OEM_BACKSPACE) { /* backspace hack */
    VGA.moveCursor(VGA.tx-1, VGA.ty);
    VGA.print(' ');
    VGA.moveCursor(VGA.tx-1, VGA.ty);
    if (keypointer != 0) {
      keybuffer[--keypointer] = '\0';
    }
    return;
  }
  if (keypointer >= KEYBUFFER_SIZE-1) {
    LOGSERIAL("the keybuffer be over. resetting\n");
    keybuffer[0] = '\0';
    keypointer = 0;
  }

  keybuffer[keypointer] = key;
  keybuffer[++keypointer] = '\0';
  
  LOGSERIAL("keybuffer: ");
  LOGSERIAL(keybuffer);
  LOGSERIAL('\n');
}

void printKey(char key, uint8_t oemCode, uint8_t mod) {
  LOGSERIAL(" key:");
  LOGSERIAL(oemCode);
  LOGSERIAL(" mod:");
  LOGSERIAL(mod);
  LOGSERIAL(" => ");
  
  if (mod & LeftCtrl) {
    LOGSERIAL("L-Ctrl ");
  }
  if (mod & LeftShift) {
    LOGSERIAL("L-Shift ");
  }
  if (mod & Alt) {
    LOGSERIAL("Alt ");
  }
  if (mod & LeftCmd) {
    LOGSERIAL("L-Cmd ");
  }
  if (mod & RightCtrl) {
    LOGSERIAL("R-Ctrl ");
  }
  if (mod & RightShift) {
    LOGSERIAL("R-Shift ");
  }
  if (mod & AltGr) {
    LOGSERIAL("AltGr ");
  }
  if (mod & RightCmd) {
    LOGSERIAL("R-Cmd ");
  } 

  if (oemCode == OEM_ENTER) {
    LOGSERIAL("Enter was pressed\n");
    return;
  }

  if (oemCode == OEM_BACKSPACE) {
    LOGSERIAL("backspace\n");
    return;
  }

  VGA.print(key);
  LOGSERIAL(key);
  LOGSERIAL('\n');
}

void setup() {
  keybuffer[0] = '\0';
  keypointer = 0;

#ifdef ENABLE_SERIAL
  Serial.begin(9600);
#endif
  LOGSERIAL("Program started\n");

#ifdef USE_NTSC
  VGA.beginNTSC();
#else
  VGA.beginPAL();
#endif

  VGA.print("p-OS\n");
  delay(200);
  VGA.print("> ");
}

void loop() {
  // Process USB tasks
  usb.Task();
}
