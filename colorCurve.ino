
#define colorcurvesize 8
int colorcurve [colorcurvesize][3] = 
  {
    { 0, 0, 0 },
    { 10, 2, 0 },
    { 31, 14, 2},
    { 58, 33, 5 },
    { 95, 63, 7 },
    { 145, 106, 14 },
    { 195, 171, 32 },
    { 255, 255, 76 }
  };

int oldbrightness = 0;

/* Same as dimThousands, but slowly increases/decreases to end up at the given value */
void goToDimThousands(int brightness) {
//  Serial.print("Dimming from ");  Serial.print(oldbrightness, DEC);
//  Serial.print(" to "); Serial.println(brightness, DEC);

  int inc = 1;
  if (brightness < oldbrightness) {
    inc = -1;
  }
  
  for (int i=oldbrightness; i!=brightness; i = i + inc) {
    dimThousands(i);
    delay(1);
  }
  
  oldbrightness = brightness;
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
  if (newRed != oldRed) {analogWrite(redPin,newRed); oldRed = newRed; }
  if (newGreen != oldGreen) {analogWrite(greenPin,newGreen); oldGreen = newGreen; }
  if (newBlue != oldBlue) {analogWrite(bluePin,newBlue); oldBlue = newBlue; }
}

