void printTouch(){
  int total = capsense.capacitiveSensor(30);
  Serial.println(total);  
}

void colorMixer() {
  int red = 128;
  int green = 128;
  int blue = 128;
  int inc = 2;
  while(true){
    while (Serial.available() > 0){
      char c = Serial.read();
      
      if (c == 'r') {
        red = constrain(red-inc, 0, 255); 
      } else if (c == 'R') {
        red = constrain(red+inc, 0, 255);
      } else if (c == 'g') {
        green = constrain(green-inc, 0, 255);
      } else if (c == 'G') {
        green = constrain(green+inc, 0, 255);
      } else if (c == 'b') {
        blue = constrain(blue-inc, 0, 255);
      } else if (c == 'B') {
        blue = constrain(blue+inc, 0, 255);
      } else {
        continue;
      }  
  
      Serial.print(c);
      Serial.print(' ');
      Serial.print(red, DEC);
      Serial.print(' ');
      Serial.print(green, DEC);
      Serial.print(' ');
      Serial.println(blue,DEC);
      stripColor(red, green, blue);
    }
  }
}
