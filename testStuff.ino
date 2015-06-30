void printTouch(){
  while(true){
    int total = capsense.capacitiveSensor(30);
    Serial.println(total);  
  }
}

void printLDR(){
  while(true){
    int light = analogRead(ldrPin);
    Serial.println(light);
    delay(200);
  }
}

void colorMixer() {
  int red = 127;
  int green = 107;
  int blue = 20;
  stripColor(red, green, blue);
  
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
  
      Serial.print(" { ");
      Serial.print(red, DEC);
      Serial.print(", ");
      Serial.print(green, DEC);
      Serial.print(", ");
      Serial.print(blue,DEC);
      Serial.println(" },");
      stripColor(red, green, blue);
    }
  }
}
