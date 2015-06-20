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
//  printTouch();
  //  colorMixer();

  // Low (off) state
  offLoop();

  // High (on) state
  onLoop();
}

void offLoop(){
  Serial.println("OFF loop");
  goToDimThousands(0);

    // Low (off) state
  while(true) {
    int activity = waitForActivity();
    if (activity == ACTIVITY_PERSON_ENTERED) {
      goToDimThousands(250);
    } else if (activity == ACTIVITY_PERSON_LEFT) {
      goToDimThousands(0);
    } if (activity == ACTIVITY_TOUCH) {
      return;
    }
  }
}

void onLoop() {
  Serial.println("ON loop");
  goToDimThousands(1000);
  while(true) {
    int activity = waitForActivity();
    if (activity == ACTIVITY_PERSON_ENTERED) {
      goToDimThousands(1000);
    } else if (activity == ACTIVITY_PERSON_LEFT) {
      goToDimThousands(250);
    } if (activity == ACTIVITY_TOUCH) {
      return;
    }
  }
}
