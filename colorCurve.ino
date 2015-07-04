
#define colorcurvesize 8
int colorcurve [colorcurvesize][3] = 
  {
    { 0, 0, 0 },
    { 10, 2, 0 },
    { 31, 14, 1},
    { 58, 33, 5 },
    { 95, 63, 7 },
    { 145, 106, 14 },
    { 195, 171, 31 },
    { 255, 255, 74 }
  };

/* Same as dimThousands, but slowly increases/decreases to end up at the given value */
void goToDimThousands(int brightness) {
  goToDimThousands(brightness,1, false);
}

/* Same as dimThousands, but slowly increases/decreases to end up at the given value */
void goToDimThousands(int brightness, int stepDelayMs, boolean checkPIR) {
//  Serial.print("Dimming from ");  Serial.print(oldbrightness, DEC);
//  Serial.print(" to "); Serial.println(brightness, DEC);

  int inc = 1;
  if (brightness < currentBrightness) {
    inc = -1;
  }
  
  for (int i=currentBrightness; i!=brightness; i = i + inc) {
    dimThousands(i);
    currentBrightness = i;
    delay(stepDelayMs);
    if (checkPIR && hasPIR()) break;
  }  
  currentBrightness = brightness;
//  Serial.print("Ended loop at brightness "); Serial.println(currentBrightness,DEC);
}


/* 0 = off, 1000 is full brightness */
void dimThousands(int brightness){
  brightness = constrain(brightness, 0, 1000);
  
  // Find the two indices
  float findex = (float) (brightness * (colorcurvesize-1)) / 1000;

  int lowindex = constrain((int) findex,0,colorcurvesize-1);
  int highindex = constrain(lowindex+1,0,colorcurvesize-1);
  
  int red = colorcurve[lowindex][0];
  int green = colorcurve[lowindex][1];
  int blue = colorcurve[lowindex][2];
    
  if (lowindex != highindex) {
    int hred   = colorcurve[highindex][0];
    int hgreen = colorcurve[highindex][1];
    int hblue  = colorcurve[highindex][2];
    
    float ratio = findex - lowindex;
    red   = (int) (0.5 + red   + (ratio * (hred   - red  )));
    green = (int) (0.5 + green + (ratio * (hgreen - green)));
    blue  = (int) (0.5 + blue  + (ratio * (hblue  - blue )));
  }
    
  stripColor(red,green,blue);
}

int oldRed = 0;
int oldGreen = 0;
int oldBlue = 0;

void stripColor(int newRed, int newGreen, int newBlue){
  boolean changed = false;
  if (newRed != oldRed) {analogWrite(redPin,newRed); oldRed = newRed; changed = true;}
  if (newGreen != oldGreen) {analogWrite(greenPin,newGreen); oldGreen = newGreen; changed = true;}
  if (newBlue != oldBlue) {analogWrite(bluePin,newBlue); oldBlue = newBlue; changed = true;}
  
  if (changed) {
//    Serial.print("R="); Serial.print(newRed,DEC);
//    Serial.print(" G="); Serial.print(newGreen,DEC);
//    Serial.print(" B="); Serial.println(newBlue,DEC);
  }
}

