#include <CapacitiveSensor.h>

#define ACTIVITY_PERSON_ENTERED  1
#define ACTIVITY_PERSON_LEFT  2
#define ACTIVITY_TOUCH  3



const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

const int redLedPin = 7;
const int yellowLedPin = 8;
const int greenLedPin = 13;

const int ldrPin = A0;

const int irPin = 6;

unsigned long onUntilMillis;

int lowRed   = 22;
int lowGreen = 8;
int lowBlue  = 0;

int highRed  = 255;
int highGreen= 245;
int highBlue = 45;

const int capsens_signal_pin = 4;
const int capsens_sense_pin = 3;

// 10M resistor between pins, add 1n4148 from sense to 5V and 10k from sense to toucharea
CapacitiveSensor capsense = CapacitiveSensor(capsens_signal_pin,capsens_sense_pin);

int currentBrightness = 0;

void setup() {
  Serial.begin(57600);
  randomSeed(analogRead(0));

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(irPin, INPUT);

  pinMode(ldrPin, INPUT);

  digitalWrite(redLedPin, HIGH);
  delay(100);
  digitalWrite(yellowLedPin, HIGH);
  delay(100);
  digitalWrite(greenLedPin, HIGH);
  delay(100);
  digitalWrite(redLedPin, LOW);
  delay(100);
  digitalWrite(yellowLedPin, LOW);
  delay(100);
  digitalWrite(greenLedPin, LOW);

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

void printBrightness(){
  Serial.print("Current LED brightness is ");
  Serial.print(currentBrightness,DEC);
  Serial.print(", and LDR is ");
  Serial.print(analogRead(ldrPin),DEC);
  Serial.println(".");
}

void offLoop(){
  
  Serial.println("--- Entering OFF state ---");
  digitalWrite(greenLedPin, LOW);
  unsigned long keepDimMs = 60000; // one minute

//  printBrightness();
  
  if (currentBrightness == 1000){
     goToDimThousands(250);
    
//     printBrightness();
     if (isTooBrightDimmed()){
       Serial.println("OFF: Too bright, turning off the lights completely.");
       goToDimThousands(0);
     }
      
  }
  
  capsense.reset_CS_AutoCal();

  unsigned long ignorePIRUntil = millis() + keepDimMs;
  digitalWrite(yellowLedPin, LOW);

  // Low (off) state
  while(true) {
    if (hasTouched()){
      digitalWrite(yellowLedPin, HIGH);
      Serial.println("OFF: User touched sensor. Exit.");
      return;
    }

    if (currentBrightness == 0 && !isDark()) {
      // Only lights the red led if somebody walks by.
      hasPIR();
      delay(50);

      // Skip rest of loop, no need to turn on ligths if it's not dark.
      continue;
    }
    
    if (hasPIR()) {
      ignorePIRUntil = millis() + keepDimMs;

      if(currentBrightness != 250) {
        Serial.println("OFF: PIR activity in the dark. Turning on the lights.");
        goToDimThousands(250);
        printBrightness();
      }
    } else if (currentBrightness == 250 && isPassed(ignorePIRUntil)) {
      Serial.println("OFF: No more PIR activity. Turning off the lights.");
      goToDimThousands(0);
    }
  }
}

void onLoop() {
  Serial.println("--- Entering ON state ---");
  digitalWrite(greenLedPin, HIGH);

  goToDimThousands(1000);
  capsense.reset_CS_AutoCal();
  
  unsigned long keepBrightMs = 300000; // five minutes

  unsigned long keepDimMs = 7200000; // two hours
  unsigned long ignorePIRUntil = millis() + keepBrightMs;
  digitalWrite(yellowLedPin, LOW);

  while(true) {
    if (hasTouched()){
      digitalWrite(yellowLedPin, HIGH);
      Serial.println("ON: User touched sensor. Exit.");
      return;
    }
     
    if (hasPIR()){
      ignorePIRUntil = millis() + keepBrightMs;
      
      if (currentBrightness != 1000) {
        Serial.println("ON: PIR activity. Switching to full brightness.");
        goToDimThousands(1000);
        capsense.reset_CS_AutoCal();
      }
    } else if (currentBrightness == 1000 && isPassed(ignorePIRUntil)) {
      Serial.println("ON: No PIR activity for a while. Dimming the lights.");
      goToDimThousands(250, 20, true);
      capsense.reset_CS_AutoCal();
      ignorePIRUntil = millis() + keepDimMs;
    } else if (currentBrightness == 250 && isTooBrightDimmed()) {      
      Serial.println("ON: Dimmed lights in ON state, and still too bright. Exit ON state.");
      return;
    } else if (currentBrightness == 250 && isPassed(ignorePIRUntil)) {      
      Serial.println("ON: No PIR activity for a long time. Exit ON state.");
      return;
    }
  }
}
