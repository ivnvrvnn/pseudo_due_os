USBHost usb;
KeyboardController keyboard(usb);
uint8_t oemCode = keyboard.getOemKey();
// enter lock test
KEY_ENTER_LOCKED = 1;
VGA.print("program v1.00");
VGA.println("");
VGA.println("Enter locked, now you can't write commands, you can only write gore arguments in program if you implement this\n");
VGA.println("");
VGA.println("Check serial monitor to see keygoreargbuffer\n");
for(int i=0;i<320;i++){
if (oemCode == 40) {
  VGA.println("Enter pressed");
  VGA.println("Text in buffer: ");
  VGA.print(keygoreargbuffer); 
  return;
};
}