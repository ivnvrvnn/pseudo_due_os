void argtest(char** args) {  
  if (args[0] == NULL || strlen(args[0]) == 0) {
    VGA.println("Usage: .argtest <number from 1 to 255>");
    return;
  }

  int number = atoi(args[0]);
  if (number <= 0) {
    VGA.println("Invalid number");
    return;
  }

  if (number > 255) {
    VGA.println("number too large");
    return;
  }

char strNumber[16];
snprintf(strNumber, sizeof(strNumber), "%d", number);

VGA.print("You entered: ");
VGA.print(strNumber);
VGA.println();
}
