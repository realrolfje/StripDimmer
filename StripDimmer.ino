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
}

void loop() {
  // Debugging stuff:
//    goToDimThousands(250);
  //  printTouch();
// printLDR();
  //  colorMixer();

  offLoop();  // Low (off) state
  onLoop();   // High (on) state
}

void offLoop(){
  
  Serial.println("--- Entering OFF state ---");
  digitalWrite(greenLedPin, LOW);

  unsigned long keepDimMs = 60000; // one minute
  int brightness = 250;  goToDimThousands(brightness);
  unsigned long ignorePIRUntil = millis() + keepDimMs;
  digitalWrite(yellowLedPin, LOW);

  // Low (off) state
  while(true) {
    if (hasTouched()){
      digitalWrite(yellowLedPin, HIGH);
      Serial.println("OFF: User touched sensor. Exit.");
      return;
    }

    if (brightness == 0 && !isDark()) {
      // Only lights the red led if somebody walks by.
      hasPIR();
      delay(50);

      // Skip rest of loop, no need to turn on ligths if it's not dark.
      continue;
    }
    
    if (hasPIR()) {
      ignorePIRUntil = millis() + keepDimMs;

      if(brightness != 250) {
        Serial.println("OFF: PIR activity in the dark. Turning on the lights.");
        brightness = 250; goToDimThousands(brightness);
      }
    } else if (brightness == 250 && isPassed(ignorePIRUntil)) {
      Serial.println("OFF: No more PIR activity. Turning off the lights.");
      brightness = 0; goToDimThousands(brightness);
    }
  }
}

void onLoop() {
  Serial.println("--- Entering ON state ---");
  digitalWrite(greenLedPin, HIGH);

  int brightness = 1000;  goToDimThousands(brightness);
  
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
      
      if (brightness != 1000) {
        Serial.println("ON: PIR activity. Switching to full brightness.");
        brightness = 1000; goToDimThousands(brightness);
      }
    } else if (brightness == 1000 && isPassed(ignorePIRUntil)) {
      Serial.println("ON: No PIR activity for a while. Dimming the lights.");
      brightness = 250; goToDimThousands(brightness, 20, true);
      ignorePIRUntil = millis() + keepDimMs;
    } else if (brightness == 250 && isPassed(ignorePIRUntil)) {      
      Serial.println("ON: No PIR activity for a long time. Exit ON state.");
      return;
    }
  }
}
