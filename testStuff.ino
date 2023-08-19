
void printTouch(){
  while(true){
    int i = 10;
    while(i--){    
      long total = capsense.capacitiveSensor(30);
      Log.Info("Touch: %l",total); 
      delay(500); 
    }
    capsense.reset_CS_AutoCal();
    Log.Info("Reset calibration of Touch sensor."); 
  }
}

void printLDR(){
  while(true){
    int light = analogRead(ldrPin);
    Log.Info("LDR value: %d",light);
    delay(200);
  }
}

void printSensors(){
  Log.Debug("RGB brightness is %d, LDR value is %d, PIR is %T",currentBrightness,analogRead(ldrPin),digitalRead(irPin));
}


void colorMixer() {
  int red = 127;
  int green = 107;
  int blue = 20;
  int white = 107;
  stripColor(red, green, blue, white);
  
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
      } else if (c == 'w') {
        blue = constrain(white-inc, 0, 255);
      } else if (c == 'W') {
        blue = constrain(white+inc, 0, 255);
      } else {
        continue;
      }  
  
      Serial.print(" { ");
      Serial.print(red, DEC);
      Serial.print(", ");
      Serial.print(green, DEC);
      Serial.print(", ");
      Serial.print(blue,DEC);
      Serial.print(", ");
      Serial.print(white,DEC);
      Serial.println(" },");
      stripColor(red, green, blue, white);
    }
  }
}
