void waitForTouch(){
  digitalWrite(ledPin,HIGH);
  while(capsense.capacitiveSensor(30) < 10000) { };
  digitalWrite(ledPin,LOW);
}

void checkIr(){
  boolean irState = digitalRead(irPin);
  digitalWrite(ledPin, irState);
  if(irState){
    onUntilMillis = millis() + irTime;
  }
}

