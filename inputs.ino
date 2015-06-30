#define touchSensitivity 5000
#define darkness 400

boolean personPresent = false;

boolean hasPIR(){
  boolean pir = digitalRead(irPin);
  digitalWrite(redLedPin, pir);
  return pir;
}

boolean isDark(){
  return analogRead(ldrPin) >= darkness;
}

boolean hasTouched() {
  int sense = capsense.capacitiveSensor(30);
  if ( sense > touchSensitivity) {
    blink();
    Serial.print("Touched sensor ");
    Serial.println(sense,DEC);
    return true;
  }
  return false;
}

void blink(){
  digitalWrite(greenLedPin,HIGH);
  delay(50);
  digitalWrite(greenLedPin,LOW);
  delay(50);
}

boolean isPassed(unsigned long targetTime) {
  unsigned long currTime = millis();
  
  if ( targetTime > 2147483647 && currTime < 640000) {
    // rollover
    return true;
  }

  if ( currTime > targetTime) {
    return true;
  }

  return false;
}

