#include <KeyboardController.h>
#include <VGA.h>

// build configuration
#define DEBUG

// keyboard keys config
#define OEM_ENTER 40
#define OEM_BACKSPACE 42

// keybuffer size
#define KEYBUFFER_SIZE 256

USBHost usb;
KeyboardController keyboard(usb);

// keybuffer
char keybuffer[KEYBUFFER_SIZE];
// change to a larger data type (uint16_t, unsigned int, etc.) if you want to expand it. the maximum uint8_t can hold is 256
uint8_t keypointer;
  
void keyPressed() {
  char key = keyboard.getKey();
  #ifdef DEBUG
  Serial.print("Pressed:");
  #endif
  uint8_t oemCode = keyboard.getOemKey();
  printKey(key, oemCode, keyboard.getModifiers());
  if (oemCode == OEM_ENTER) { /* enter */
    VGA.write('\n');
    commandProcessor();
    if (keypointer != 0) {
      #ifdef DEBUG
      Serial.print("resetting the buffer. was: ");
      Serial.println(keybuffer);
      #endif
      keypointer = 0;
      keybuffer[0] = '\0';
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
  keybuffer[keypointer] = key;
  keybuffer[++keypointer] = '\0';
  #ifdef DEBUG
  Serial.println(keybuffer);
  #endif
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
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Program started\n");
  #endif
  VGA.beginPAL();
  VGA.print("keyboard capturer\n");
  delay(200);
}

void loop() {
  // Process USB tasks
  usb.Task();
}

void commandProcessor() {
  char prefix = {"."};
  if(strcmp(keybuffer != prefix) {
   if (strcmp(keybuffer, ".circle") == 0) {
      VGA.drawCircle(12, 12, 5, 5);
      VGA.fillCircle(12, 12, 5, 5);
   } else if (strcmp(keybuffer, ".test") == 0) {
      VGA.print("This is a test");
   } else {
     VGA.print("Command not found");
     return;
   }
  } else
  return;
  keybuffer[0] = '\0';
  keypointer = 0;
}
