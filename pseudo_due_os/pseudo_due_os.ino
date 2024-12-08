// pseudo os for due

// including required libraries
#include <KeyboardController.h>
#include <VGA.h>
#include <complex>
// hardware configuration
#define RESET 2
// build configuration
#define DEBUG

// keyboard keys config
#define OEM_ENTER 40
#define OEM_BACKSPACE 42

// keybuffer size
#define KEYBUFFER_SIZE 256
// lastcmd size
#define LASTCMD_SIZE 256 

USBHost usb;
KeyboardController keyboard(usb);

bool KEY_ENTER_LOCKED;

// keybuffer
char keybuffer[KEYBUFFER_SIZE];
char keygoreargbuffer[KEYBUFFER_SIZE];

// last command buffer

// change to a larger data type (uint16_t, unsigned int, etc.) if you want to expand it. the maximum uint8_t can hold is 256
uint8_t keypointer;
uint16_t c;

// 1. cpu is halting if i use this 
// extern "C" char* sbrk(int incr);

// int freeRam() {
//   char top;
//   return &top - reinterpret_cast<char*>(sbrk(0));
// }

void keyPressed() {
  char key = keyboard.getKey();
  #ifdef DEBUG
  Serial.print("Pressed:");
  #endif
  uint8_t oemCode = keyboard.getOemKey();
  printKey(key, oemCode, keyboard.getModifiers());
  if (oemCode == OEM_ENTER) { /* enter */
   if (KEY_ENTER_LOCKED == 1){ // buffer disabled
    } else {
    VGA.write('\n');
    commandProcessor();
    if (keypointer != 0) {
      #ifdef DEBUG
      Serial.print("resetting the buffer. was: ");
      Serial.println(keybuffer);
      #endif
      keypointer = 0;
      // needs to be rewritten
//    char lastcmd[LASTCMD_SIZE] = keybuffer;
      keybuffer[0] = '\0';
      }
    }
    return;
  }
  if (oemCode == OEM_BACKSPACE) {
    //VGA.write('\b');
    if (keypointer != 0) {
      keybuffer[--keypointer] = '\0';
    }
    return;
  }
  if (keypointer >= KEYBUFFER_SIZE-1) {
    #ifdef DEBUG
    Serial.println("the keybuffer be over. resetting");
    #endif
    keybuffer[0] = '\0';
    keypointer = 0;
  }
  if (KEY_ENTER_LOCKED == 1){ // buffer reset disabled
    keygoreargbuffer[keypointer] = key;
    keygoreargbuffer[++keypointer] = '\0';
    #ifdef DEBUG
    Serial.println(keygoreargbuffer);
    #endif
  } else {
    keybuffer[keypointer] = key;
    keybuffer[++keypointer] = '\0';
    #ifdef DEBUG
    Serial.println(keybuffer);
    #endif
  }
}

//void keyReleased() {
//  Serial.print("Released: ");
//  printKey();
//}

void printKey(char key, uint8_t oemCode, uint8_t mod) {
  #ifdef DEBUG
  Serial.print(" key:");
  Serial.print(oemCode);
  Serial.print(" mod:");
  Serial.print(mod);
  Serial.print(" => ");
  #endif
  
  if (mod & LeftCtrl) {
    #ifdef DEBUG
    Serial.print("L-Ctrl ");
    #endif
  }
  if (mod & LeftShift) {
    #ifdef DEBUG
    Serial.print("L-Shift ");
    #endif
  }
  if (mod & Alt) {
    #ifdef DEBUG
    Serial.print("Alt ");
    #endif
  }
  if (mod & LeftCmd) {
    #ifdef DEBUG
    Serial.print("L-Cmd ");
    #endif
  }
  if (mod & RightCtrl) {
    #ifdef DEBUG
    Serial.print("R-Ctrl ");
    #endif
  }
  if (mod & RightShift) {
    #ifdef DEBUG
    Serial.print("R-Shift ");
    #endif
  }
  if (mod & AltGr) {
    #ifdef DEBUG
    Serial.print("AltGr ");
    #endif
  }
  if (mod & RightCmd) {
    #ifdef DEBUG
    Serial.print("R-Cmd ");
    #endif
  } 

  if (oemCode == OEM_ENTER) {
    #ifdef DEBUG
    Serial.println("Enter was pressed");
    #endif
    return;
};

  if (oemCode == OEM_BACKSPACE) {
    #ifdef DEBUG
    Serial.println("backspace");
    #endif
    return;
  };

 // using VGA.print instead of VGA.write
  VGA.print(key);
  Serial.println(key);
}

void setup() {
  keybuffer[0] = '\0';
  keypointer = 0;
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Program started\n");
  #endif
  VGA.beginPAL();
  VGA.print("p-OS\n");
  delay(200);
}

// bool ps2test_started = 0; unused code
bool newton_started = 0;

void loop() {
  // Process USB tasks
  usb.Task();
}

void reset() {
      #ifdef DEBUG
      Serial.println("RESETTING");
      #endif
      digitalWrite(RESET, LOW);
      VGA.println("If you see this text, you didn't connect reset");
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
   } else {
     VGA.print("Command not found\n");
     return;
   }
  } else 
  return;
  keybuffer[0] = '\0';
  keypointer = 0;
}
