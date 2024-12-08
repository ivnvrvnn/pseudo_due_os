// pseudo os for due

// including required libraries
#include <KeyboardController.h>
#include <VGA.h>
#include <complex>

#include "config.h"

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
  VGA.beginPAL();
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

void reset() {
      #ifdef LOG_OUTPUT_TO_SERIAL
      Serial.println("RESETTING");
      #endif
      setup(); // do not use reset, system will crash
}
void commandProcessor() {
  if(strcmp(keybuffer , ".") != 0) { // it ignoring prefix, needs to fix
   if (strcmp(keybuffer, ".circle") == 0) {
      #include "programs\circle.h"
   } else if (strcmp(keybuffer, ".test") == 0) {
      #include "programs\test.h" 
   } else if (strcmp(keybuffer, ".reset") == 0) {
      reset();
   } else if (strcmp(keybuffer, ".newton_demo") == 0) {
     #include "programs\newton_demo.h"
     // 1. cpu is halting if i use this 
  //  } else if (strcmp(keybuffer, ".free") == 0) {
  //    VGA.print(F("- SRAM left: "));
  //    VGA.println(freeRam());
   } else if (strcmp(keybuffer, ".elt") == 0) {
     #include "programs\enter_lock_test.h"
  //  } else if (strcmp(keybuffer, ".cleargfx") == 0) {
  //   VGA.clear();
   } else if (strcmp(keybuffer, ".curtest") == 0) {
    // cursor position test
    VGA.print("X: ");
    VGA.print(Xcursor);
    VGA.println("Y:");
    VGA.print(Ycursor);
   } else {
     VGA.print("Command not found\n");
     return;
   }
  } else 
  return;
  keybuffer[0] = '\0';
  keypointer = 0;
}

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