#define touchSensitivity 7000

boolean personPresent = false;

int waitForActivity(){
  while(true){
    int sense = capsense.capacitiveSensor(30);
//    Serial.println(sense);  

    if ( sense > touchSensitivity) {
      personPresent = true;
      blink();
      Serial.print("Touched sensor ");
      Serial.println(sense,DEC);
      return ACTIVITY_TOUCH;
    }
    
    boolean pirDetected = digitalRead(irPin);
    if (!personPresent && pirDetected){
      blink();
      blink();
      Serial.println("Person entered the room");
      personPresent = true;
      return ACTIVITY_PERSON_ENTERED;
    } else if (personPresent && !pirDetected) {
      personPresent = false;
      blink();
      blink();
      blink();
      Serial.println("Person left the room");
      return ACTIVITY_PERSON_LEFT;
    }
  }
}

void blink(){
  digitalWrite(ledPin,HIGH);
  delay(50);
  digitalWrite(ledPin,LOW);
  delay(50);
}

void waitForTouch(){
  int sense = 0;
  while((sense = capsense.capacitiveSensor(30)) < touchSensitivity) { };
  digitalWrite(ledPin,HIGH);
  Serial.println(sense,DEC);
  digitalWrite(ledPin,LOW);
}

void checkIr(){
  boolean irState = digitalRead(irPin);
  digitalWrite(ledPin, irState);
  if(irState){
    onUntilMillis = millis() + irTime;
  }
}

