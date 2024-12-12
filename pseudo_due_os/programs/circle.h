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