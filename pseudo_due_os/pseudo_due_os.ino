// pseudo os for due

// including required libraries
#include <KeyboardController.h>
#include <VGA.h>
#include <complex>
#include <string.h>

#include "config.h"
#include "prglist.h"

#define MAX_COMMAND_LENGTH 32
#define MAX_ARGUMENTS 8

USBHost usb;
KeyboardController keyboard(usb);

bool KEY_ENTER_LOCKED;
bool NEW_LINE;

// keybuffer
char keybuffer[KEYBUFFER_SIZE];
char keygoreargbuffer[KEYBUFFER_SIZE];
#ifdef KEYPOINTER_SUPPORT_LARGER_DATA_TYPE
uint16_t keypointer;
#else
uint8_t keypointer;
#endif

// inital position
byte Ycursor = 3;
byte Xcursor = 2;

// 1. cpu is halting if i use this 
// extern "C" char* sbrk(int incr);

// int freeRam() {
//   char top;
//   return &top - reinterpret_cast<char*>(sbrk(0));
// }

typedef struct Command {
  const char* name;
  void (*handler)(char**);
} Command;

void reset(char** args);
void circle(char** args);

//#include "programs\newton_demo.h"

const Command commands[] = {
  {".circle", &circle},
//{"test", &test},
  {".reset", &reset},
//{"newton_demo", &newtonDemo},
//{"elt", &enterLockTest},
//{"curtest", &curTest},
//{"randomtest", &randomTest},
//{".argtest", &argtest}
};

size_t numCommands = sizeof(commands) / sizeof(Command);

void processArguments(char** argv, size_t argc) {
  for (size_t i = 0; i < numCommands; ++i) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      commands[i].handler(argv + 1);
      return;
    }
  }
  VGA.print("Command not found: ");
  VGA.println(argv[0]);
}

void commandProcessor() {
  char buffer[MAX_COMMAND_LENGTH];
  strcpy(buffer, keybuffer);

  char* token = strtok(buffer, " ");
  while (token != NULL) {
    char* argv[MAX_ARGUMENTS];
    size_t argc = 0;
    
    argv[argc++] = token;
    while ((token = strtok(NULL, " ")) != NULL && argc < MAX_ARGUMENTS) {
      argv[argc++] = token;
    }
    
    processArguments(argv, argc);
    
    token = strtok(NULL, ";");
  }

  keybuffer[0] = '\0';
  keypointer = 0;
}

bool checkCommandPrefix(const char* command, const char* prefix) {
  int prefixLength = strlen(prefix);
  if (prefixLength > strlen(command)) {
    return false;
  }
  for (int i = 0; i < prefixLength; ++i) {
    if (command[i] != prefix[i]) {
      return false;
    }
  }
  return true;
}

void keyPressed() {
  char key = keyboard.getKey();
  #ifdef LOG_OUTPUT_TO_SERIAL
  Serial.print("Pressed:");
  #endif
  if (NEW_LINE == 1){
  VGA.print("> ");
  }
  uint8_t oemCode = keyboard.getOemKey();
  printKey(key, oemCode, keyboard.getModifiers());
  if (oemCode == OEM_ENTER) { /* enter */
   if (KEY_ENTER_LOCKED == 1){ // buffer disabled
    } else {
    VGA.write('\n');
    commandProcessor();
    if (keypointer != 0) {
      #ifdef LOG_OUTPUT_TO_SERIAL
      Serial.print("resetting the buffer. was: ");
      Serial.println(keybuffer);
      #endif
      keypointer = 0;
      // needs to be rewritten
//    char lastcmd[LASTCMD_SIZE] = keybuffer;
      keybuffer[0] = '\0';
      NEW_LINE = 1;
      }
      NEW_LINE = 0;
      VGA.print("> ");
      Xcursor = 2;
    }
    return;
  }
  // bad backspace implementation
  if (oemCode == OEM_BACKSPACE) {
    Xcursor = Xcursor - 2;
    VGA.moveCursor(Xcursor, Ycursor);
    VGA.print(" ");
    Xcursor = Xcursor - 1;
    Ycursor = Ycursor - 2;
    if (keypointer != 0) {
      keybuffer[--keypointer] = '\0';
    }
    return;
  }
  if (keypointer >= KEYBUFFER_SIZE-1) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.println("the keybuffer be over. resetting");
    #endif
    keybuffer[0] = '\0';
    keypointer = 0;
  }
  if (KEY_ENTER_LOCKED == 1){ // buffer reset disabled
    keygoreargbuffer[keypointer] = key;
    keygoreargbuffer[++keypointer] = '\0';
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.println(keygoreargbuffer);
    #endif
  } else {
    keybuffer[keypointer] = key;
    keybuffer[++keypointer] = '\0';
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.println(keybuffer);
    #endif
  }
}

//void keyReleased() {
//  Serial.print("Released: ");
//  printKey();
//}

void printKey(char key, uint8_t oemCode, uint8_t mod) {
  #ifdef LOG_OUTPUT_TO_SERIAL
  Serial.print(" key:");
  Serial.print(oemCode);
  Serial.print(" mod:");
  Serial.print(mod);
  Serial.print(" => ");
  #endif
  
  if (mod & LeftCtrl) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("L-Ctrl ");
    #endif
  }
  if (mod & LeftShift) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("L-Shift ");
    #endif
  }
  if (mod & Alt) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("Alt ");
    #endif
  }
  if (mod & LeftCmd) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("L-Cmd ");
    #endif
  }
  if (mod & RightCtrl) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("R-Ctrl ");
    #endif
  }
  if (mod & RightShift) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("R-Shift ");
    #endif
  }
  if (mod & AltGr) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("AltGr ");
    #endif
  }
  if (mod & RightCmd) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.print("R-Cmd ");
    #endif
  } 

  if (oemCode == OEM_ENTER) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.println("Enter was pressed");
    #endif
    return;
};

  if (oemCode == OEM_BACKSPACE) {
    #ifdef LOG_OUTPUT_TO_SERIAL
    Serial.println("backspace");
    #endif
    return;
  };

 // using VGA.print instead of VGA.write
  Xcursor = Xcursor + 1;
  VGA.print(key);
  Serial.println(key);
}

void setup() {
  keybuffer[0] = '\0';
  keypointer = 0;
  #ifdef ENABLE_SERIAL
  Serial.begin(9600);
  #endif
  #ifdef LOG_OUTPUT_TO_SERIAL
  Serial.print("Program started\n");
  #endif
  #ifdef USE_NTSC
  VGA.beginNTSC();
  #else
  VGA.beginPAL();
  #endif
  VGA.print("p-OS\n");
  delay(200);
  VGA.print("> ");
}

// bool ps2test_started = 0; unused code
bool newton_started = 0;

void loop() {
  // Process USB tasks
  usb.Task();
//cursorBlink();
}

void reset(char** args) {
      #ifdef LOG_OUTPUT_TO_SERIAL
      Serial.println("RESETTING");
      #endif
      setup(); // do not use reset, system will crash
}

void circle(char** args) {
  if (args[0] == NULL || strlen(args[0]) == 0) {
    VGA.println("Usage: .circle <radius>");
    return;
  }

  int radius = atoi(args[0]);
  if (radius <= 0) {
    VGA.println("Invalid radius");
    return;
  }

  if (radius > 255) {
    VGA.println("Radius too large");
    return;
  }


  char strRadius[16];
  snprintf(strRadius, sizeof(strRadius), "%d", radius);
  
  VGA.println("Drawing a circle with radius: ");
  VGA.println(strRadius);
  VGA.drawCircle(32, 32, radius, 1);
  VGA.fillCircle(32, 32, radius, 1);
}

// idk

// void cursorGet(char XY) {
//   if (XY == "X") {

//    return curX
//   }
// }

// don't use this, it doesn't work

// void cursorBlink() {
//   delay(cursorBlinkDelay);
//   VGA.print("_");
//   Xcursor = Xcursor - 1;
//   VGA.moveCursor(Xcursor, Ycursor);
//   VGA.print(" ");
// }