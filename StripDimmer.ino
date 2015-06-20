#include <CapacitiveSensor.h>

#define ACTIVITY_PERSON_ENTERED  1
#define ACTIVITY_PERSON_LEFT  2
#define ACTIVITY_TOUCH  3


const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

const int ledPin = 13;

const int irPin = 6;

const int irTime = 10000;
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

  pinMode(ledPin, OUTPUT);
  pinMode(irPin, INPUT);

  stripColor(0,0,0);
}

void loop() {
  // Debugging stuff:
  //  printTouch();
  //  colorMixer();

  offLoop();  // Low (off) state
  onLoop();   // High (on) state
}

void offLoop(){
  Serial.println("--- Entering OFF state ---");
  int keepDimMs = 5000;
  int brightness = 250;  goToDimThousands(brightness);
  long ignorePIRUntil = millis() + keepDimMs;

  // Low (off) state
  while(true) {
    if (hasTouched()){
      Serial.println("OFF: User touched sensor. Exit.");
      return;
    }

    if (hasPIR()) {
      ignorePIRUntil = millis() + keepDimMs;
      if(brightness != 250) {
        Serial.println("OFF: PIR activity. Turning on the lights.");
        brightness = 250; goToDimThousands(brightness);
      }
    } else if (brightness == 250 && millis() > ignorePIRUntil) {
        Serial.println("OFF: No more PIR activity. Turning off the lights.");
      brightness = 0; goToDimThousands(brightness);
    }
  }
}

void onLoop() {
  Serial.println("--- Entering ON state ---");
  int brightness = 1000;  goToDimThousands(brightness);
  
  int keepBrightMs = 5000;
  int keepDimMs = 5000;
  long ignorePIRUntil = millis() + keepBrightMs;

  while(true) {
    if (hasTouched()){
      Serial.println("ON: User touched sensor. Exit.");
      return;
    }
     
    if (hasPIR()){
      ignorePIRUntil = millis() + keepBrightMs;
      
      if (brightness != 1000) {
        Serial.println("ON: PIR activity. Switching to full brightness.");
        brightness = 1000; goToDimThousands(brightness);
      }
    } else if (brightness == 1000 && millis() > ignorePIRUntil) {
      Serial.println("ON: No PIR activity for a while. Dimming the lights.");
      ignorePIRUntil = millis() + keepDimMs;
      brightness = 250; goToDimThousands(brightness);
    } else if (brightness == 250 && millis() > ignorePIRUntil) {      
      Serial.println("ON: No PIR activity for a long time. Exit ON state.");
      return;
    }
  }
}
