#include <CapacitiveSensor.h>

const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

int lowRed   = 22;
int lowGreen = 5;
int lowBlue  = 0;

int highRed  = 255;
int highGreen= 245;
int highBlue = 45;

const int capsens_signal_pin = 4;
const int capsens_sense_pin = 3;


// 10M resistor between pins, add 1n4148 from sense to 5V and 10k from sense to toucharea
CapacitiveSensor capsense = CapacitiveSensor(capsens_signal_pin,capsens_sense_pin);

void setup() {
//  Serial.begin(57600);
  
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 

  stripColor(0,0,0);
}

void loop() {
//  printTouch();
//  return;

    dimLow(0);
    

    waitForTouch();
    dim1();
    waitForTouch();
    dim2();
    waitForTouch();
    dim3();
    waitForTouch();
    dim4();

//  while (Serial.available() > 0){
//    char c = Serial.read();
//    
//    if (c == 'r') {
//      red = constrain(red-inc, 0, 255); 
//    } else if (c == 'R') {
//      red = constrain(red+inc, 0, 255);
//    } else if (c == 'g') {
//      green = constrain(green-inc, 0, 255);
//    } else if (c == 'G') {
//      green = constrain(green+inc, 0, 255);
//    } else if (c == 'b') {
//      blue = constrain(blue-inc, 0, 255);
//    } else if (c == 'B') {
//      blue = constrain(blue+inc, 0, 255);
//    } else {
//      continue;
//    }  
//
//    Serial.print(c);
//    Serial.print(' ');
//    Serial.print(red, DEC);
//    Serial.print(' ');
//    Serial.print(green, DEC);
//    Serial.print(' ');
//    Serial.println(blue,DEC);
//    stripColor(red, green, blue);
//  }
  
  
}
void printTouch(){
  int total = capsense.capacitiveSensor(30);
  Serial.println(total);  
}


void waitForTouch(){
  while(capsense.capacitiveSensor(30) < 7000) { };
}

void dim1(){
    for (int i=0; i<100; i++) {
      dimLow((float) i / 100);
      delay(15);
    }
}

void dim2(){
      for (int j=0; j<=100; j++) {
      dimHigh((float) j / 100);
      delay(15);
    }   
}

void dim3(){
      for (int j=100; j>=0; j--) {
      dimHigh((float) j / 100);
      delay(15);
    }   
}

void dim4(){
      for (int j=100; j>=0; j--) {
      dimLow((float) j / 100);
      delay(15);
    }   
}

int oldRed = 0;
int oldGreen = 0;
int oldBlue = 0;

void stripColor(int newRed, int newGreen, int newBlue){
  if (newRed != oldRed) {analogWrite(redPin,newRed); oldRed = newRed; }
  if (newGreen != oldGreen) {analogWrite(greenPin,newGreen); oldGreen = newGreen; }
  if (newBlue != oldBlue) {analogWrite(bluePin,newBlue); oldBlue = newBlue; }
}

void dimHigh(float dim) {
  int red   = calcDim(lowRed,highRed,dim);
  int green = calcDim(lowGreen,highGreen,dim);
  int blue  = calcDim(lowBlue,highBlue,dim);
  stripColor(red, green, blue);
}

void dimLow(float dim){
  int red   = calcDim(0,lowRed,dim);
  int green = calcDim(0,lowGreen,dim);
  int blue  = calcDim(0,lowBlue,dim);
  stripColor(red, green, blue);
}

int calcDim(int low, int high, float dim) {
  return (0.5 + low + dim * (high-low));
}


