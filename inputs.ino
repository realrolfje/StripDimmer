
boolean personPresent = false;

boolean hasPIR(){
  boolean pir = digitalRead(irPin);
  digitalWrite(redLedPin, pir);
  return pir;
}

boolean isTooBrightDimmed(){
  return analogRead(ldrPin) <= darknessWithDimmedLights;
}


boolean isDark(){
  return analogRead(ldrPin) >= darkness;
}

boolean hasTouched() {
  long sense = capsense.capacitiveSensor(30);
  boolean touched = ( sense < 0 || sense > touchSensitivity);
  digitalWrite(yellowLedPin, touched); 
  
  if (touched){
    Log.Debug("Touch detected at level %d.",sense);
  }
  
  return touched;
}

boolean isPassed(unsigned long targetTime) {
  unsigned long currTime = millis();
  
  if ( targetTime > 2147483647 && currTime < 640000) {
    // rollover
    Log.Debug("Current time rolled over, target time of %l has passed.",targetTime);
    return true;
  }

  if ( currTime > targetTime) {
    Log.Debug("Target time of %l has passed.",targetTime);
    return true;
  }

  return false;
}

