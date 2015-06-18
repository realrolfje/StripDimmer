
const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

int red = 0;
int green = 0;
int blue = 0;

int lowRed   = 22;
int lowGreen = 2;
int lowBlue  = 0;

int highRed  = 255;
int highGreen= 145;
int highBlue = 45;

int inc = 2;

void setup() {
  Serial.begin(57600);
  
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  
  stripColor(red,green,blue);
//  stripColor(10,10,10);
  
}

void loop() {

    dimHigh(0);
//    dimLow(0);
    delay(1000);

    for (int i=0; i<=100; i++) {
      dimHigh((float) i / 100);
//      dimLow((float) i / 100);
      delay(20);
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

void stripColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
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
