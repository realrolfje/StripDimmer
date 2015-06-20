#define touchSensitivity 7000

boolean personPresent = false;

boolean hasPIR(){
  return digitalRead(irPin);
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
  digitalWrite(ledPin,HIGH);
  delay(50);
  digitalWrite(ledPin,LOW);
  delay(50);
}

