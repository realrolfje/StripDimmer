#include <CapacitiveSensor.h>
#include <Logging.h>

//
// Hardware configuration
//
const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

const int redLedPin = 7;
const int yellowLedPin = 8;
const int greenLedPin = 13;

const int ldrPin = A0;

const int irPin = 6;

const int capsens_signal_pin = 4;
const int capsens_sense_pin = 3;

//
// Configuration and constants
//

// Uncomment to switch off debugging
 #define DEBUG

const int touchSensitivity = 1800;       // Lower value is higher Touch sensitivity.
const int darkness = 150;                // LDR level at which lights will go on at PIR activity in OFF state.
const int darknessWithDimmedLights = 90; // LDR level at which lights will go completely off when going from ON to OFF state.

const int maxBrightness = 1000;          // Index used to set the highest RGB LED brightness.
const int dimmedBrightness = 250;        // Index used to set dimmed RGB LED brightness.

#ifdef DEBUG
  #define LOGLEVEL LOG_LEVEL_DEBUG
  const unsigned long keepBrightMs =  3000L; // Max Full-Brightness time without PIR activity
  const unsigned long keepOnDimMs  = 10000L; // Max Dimmed time when ON without PIR activity
  const unsigned long keepOffDimMs =  6000L; // Max Dimmed time when OFF without PIR activity
#else 
  #define LOGLEVEL LOG_LEVEL_INFO
  const unsigned long keepBrightMs =  300000L; // Max Full-Brightness time without PIR activity
  const unsigned long keepOnDimMs  = 7200000L; // Max Dimmed time when ON without PIR activity
  const unsigned long keepOffDimMs =   60000L; // Max Dimmed time when OFF without PIR activity
#endif

//
// Local variables
//
unsigned long ignorePIRUntil = 0;

// 10M resistor between pins, add 1n4148 from sense to 5V and 10k from sense to toucharea
CapacitiveSensor capsense = CapacitiveSensor(capsens_signal_pin,capsens_sense_pin);

int currentBrightness = 0;

void setup() {
  Log.Init(LOGLEVEL, 57600L);

  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,  OUTPUT);

  pinMode(redLedPin,    OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin,  OUTPUT);

  pinMode(irPin,  INPUT);
  pinMode(ldrPin, INPUT);

  blinkSignalLeds();
  stripColor(0,0,0);
  
  capsense.set_CS_AutocaL_Millis(5000);
  capsense.set_CS_Timeout_Millis(200);

  goToDimThousands(0); // Init LEDs to off state.
}

void loop() {
  // Debugging stuff:
  
//  while(true){
//    hasTouched();
//  }
  
//  goToDimThousands(250);
//  printTouch();
//  printLDR();
//  colorMixer();

  offLoop();           // Low (off) state
  onLoop();            // High (on) state
}

void offLoop(){ 
  Log.Info("--- Entering OFF state ---");
  digitalWrite(greenLedPin, LOW);

  if (currentBrightness >= dimmedBrightness){
     goToDimThousands(dimmedBrightness);

     if (isTooBrightDimmed()){
       Log.Info("OFF: Too bright, turning off the lights completely.");
       goToDimThousands(0);
     }
  }
  
  capsense.reset_CS_AutoCal();

  // Low (off) state
  while(true) {
    if (hasTouched()){
      Log.Info("OFF: User touched sensor. Exit.");
      return;
    }

    if (currentBrightness == 0 && !isDark()) {
      // Call to hasPIR() to turn the red indicator led on or off
      hasPIR();
      // Skip rest of loop, no need to turn on ligths if it's not dark.
      continue;
    }
    
    if (hasPIR()) {
      ignorePIRUntil = millis() + keepOffDimMs;

      if(currentBrightness != dimmedBrightness) {
        Log.Info("OFF: PIR  in the dark. Turning on the lights.");
        goToDimThousands(dimmedBrightness);
      }
    } else if (currentBrightness == dimmedBrightness && isPassed(ignorePIRUntil)) {
      Log.Info("OFF: No more PIR activity. Turning off the lights.");
      goToDimThousands(0);
    }
  }
}

void onLoop() {
  Log.Info("--- Entering ON state ---");
  digitalWrite(greenLedPin, HIGH);

  goToDimThousands(maxBrightness);
  capsense.reset_CS_AutoCal();
  
  ignorePIRUntil = millis() + keepBrightMs;
 
  while(true) {
    if (hasTouched()){
      Log.Info("ON: User touched sensor. Exit.");
      return;
    }
     
    if (hasPIR()){
      ignorePIRUntil = millis() + keepBrightMs;
      
      if (currentBrightness != maxBrightness) {
        Log.Info("ON: PIR activity. Switching to full brightness.");
        goToDimThousands(maxBrightness);
        capsense.reset_CS_AutoCal();
      }
    } else if (currentBrightness == maxBrightness && isPassed(ignorePIRUntil)) {
      Log.Info("ON: No PIR activity for a while. Dimming the lights.");
      goToDimThousands(dimmedBrightness, 20, true);
      capsense.reset_CS_AutoCal();
      ignorePIRUntil = millis() + keepOnDimMs;
      
      if (currentBrightness == dimmedBrightness && isTooBrightDimmed()) {      
        Log.Info("ON: Dimmed lights in ON state, and still too bright. Exit ON state.");
        return;
      }
    } else if (currentBrightness == dimmedBrightness && isPassed(ignorePIRUntil)) {      
      Log.Info("ON: No PIR activity for a long time. Exit ON state.");
      return;
    }
  }
}

void blinkSignalLeds(){
  const int delayms = 50;
  for(int i=1; i<=3; i++) {
    digitalWrite(redLedPin, HIGH);    delay(delayms);
    digitalWrite(yellowLedPin, HIGH); delay(delayms);
    digitalWrite(greenLedPin, HIGH);  delay(delayms);
    digitalWrite(redLedPin, LOW);     delay(delayms);
    digitalWrite(yellowLedPin, LOW);  delay(delayms);
    digitalWrite(greenLedPin, LOW);   delay(delayms);
  }
}
