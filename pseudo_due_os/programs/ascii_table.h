VGA.println("ASCII Table ~ Character Map");  
byte thisByte = 33; 
for(int j=0;j<240;j++){ 
  VGA.write(thisByte);    
  VGA.print(", dec: "); 
  VGA.print(thisByte);      
  VGA.print(", hex: "); 
  VGA.print(thisByte, HEX);     
  VGA.print(", oct: "); 
  VGA.print(thisByte, OCT);     
  VGA.print(", bin: "); 
  VGA.println(thisByte, BIN);   
  thisByte++;  
  
  delay(300);
}