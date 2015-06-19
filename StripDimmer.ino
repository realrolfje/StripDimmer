#include <TimerOne.h>

const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

int lowRed   = 22*3;
int lowGreen = 2*3;
int lowBlue  = 0;

int highRed  = 255;
int highGreen= 145;
int highBlue = 45;

int inc = 2;

void setup() {
  Serial.begin(57600);
  
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  
  Timer1.initialize(1);
  Timer1.attachInterrupt(ledpwm);

  stripColor(0,0,0);
//  stripColor(10,10,10);
  
}

volatile int red=1023;
volatile int green=1023;
volatile int blue=1023;

int countperiod=1023;
int countred=0;
int countgreen=0;
int countblue=0;

void ledpwm(){
  if (countperiod == 0){
    countperiod = 1024;
    
    countred = red;     if (countred > 0)   digitalWrite(redPin, HIGH);
    countgreen = green; if (countgreen > 0) digitalWrite(greenPin, HIGH);
    countblue = blue;   if (countblue > 0) digitalWrite(bluePin, HIGH);
  } else {
    countperiod--;
  }
  
  if (countred == 0) {
    digitalWrite(redPin, LOW);
  } else {
    // decreases past 0 but no problem since int is bigger than 1024 bits period
    countred--;
  }    
  
  if (countgreen == 0) {
    digitalWrite(greenPin, LOW);
  } else {
    countgreen--;
  }    

  if (countblue == 0) {
    digitalWrite(bluePin, LOW);
  } else {
    countblue--;
  }    
}

void loop() {
    stripColor(1,0,0);
    delay(500);
    stripColor(2,0,0);
    delay(500);
    stripColor(0,1,0);
    delay(500);
    stripColor(0,2,0);
    delay(500);
    stripColor(0,0,1);
    delay(500);
    stripColor(0,0,2);
    delay(500);
    return;
  
  
//    dimHigh(0);
    dimLow(0);
    delay(1000);

    for (int i=0; i<=1000; i++) {
//      dimHigh((float) i / 100);
      dimLow((float) i / 1000);
//      delay(1);
    }
    
    delay(1000);
  
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

void stripColor(int newRed, int newGreen, int newBlue){
  red = newRed;
  green = newGreen;
  blue = newBlue;  
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
