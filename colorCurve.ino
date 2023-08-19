
#define colorcurvesize 8
int colorcurve [colorcurvesize][4] = 
  {
    { 0, 0, 0,  0 },
    { 10, 2, 0, 6 },
    { 31, 14, 1, 15 },
    { 58, 33, 5, 32 },
    { 95, 63, 7, 55 },
    { 145, 106, 14, 88 },
    { 195, 171, 31, 132 },
    { 255, 255, 74, 194 }
  };

/* Same as dimThousands, but slowly increases/decreases to end up at the given value */
void goToDimThousands(int brightness) {
  goToDimThousands(brightness,1, false);
}

/* Same as dimThousands, but slowly increases/decreases to end up at the given value */
void goToDimThousands(int brightness, int stepDelayMs, boolean checkPIR) {
  Log.Debug("Dimming from %d to %d.",currentBrightness,brightness);

  int inc = 1;
  if (brightness < currentBrightness) {
    inc = -1;
  }
  
  while (currentBrightness != brightness) {
    currentBrightness = currentBrightness + inc;
    dimThousands(currentBrightness);
    if (checkPIR && hasPIR()) {
      Log.Debug("Dimming interrupted at %d by PIR.",currentBrightness);
      break;
    }
    delay(stepDelayMs);
  }  
  Log.Debug("Dimming finished, now at %d.",currentBrightness);
  printSensors();
}


/* 0 = off, 1000 is full brightness */
void dimThousands(int brightness){
  brightness = constrain(brightness, 0, 1000);
  
  // Find the two indices
  float findex = (float) (brightness * (colorcurvesize-1)) / 1000;
  int lowindex = constrain((int) findex,0,colorcurvesize-1);
  int highindex = constrain(lowindex+1,0,colorcurvesize-1);
  
  // RGB color from the lower index in the array
  int red = colorcurve[lowindex][0];
  int green = colorcurve[lowindex][1];
  int blue = colorcurve[lowindex][2];
  int white = colorcurve[lowindex][3];
    
  if (lowindex != highindex) {
    // lowindex and highindex are not the same so we need to interpolate
    
    // RGB color from the higher index in the array
    int hred   = colorcurve[highindex][0];
    int hgreen = colorcurve[highindex][1];
    int hblue  = colorcurve[highindex][2];
    int hwhite = colorcurve[highindex][3];
    
    // The Lineair interpolated color between the two array points
    float ratio = findex - lowindex;
    red   = (int) (0.5 + red   + (ratio * (hred   - red  )));
    green = (int) (0.5 + green + (ratio * (hgreen - green)));
    blue  = (int) (0.5 + blue  + (ratio * (hblue  - blue )));
    white = (int) (0.5 + white + (ratio * (hwhite - white )));
  }
    
  stripColor(red,green,blue, white);
}

int oldRed = 0;
int oldGreen = 0;
int oldBlue = 0;
int oldWhite = 0;

void stripColor(int newRed, int newGreen, int newBlue, int newWhite) {
  boolean changed = false;

  if (newRed != oldRed)     { oldRed   = newRed;   changed = true; }
  if (newGreen != oldGreen) { oldGreen = newGreen; changed = true; }
  if (newBlue != oldBlue)   { oldBlue  = newBlue;  changed = true; }
  if (newWhite != oldWhite) { oldWhite = newWhite; changed = true; }

  uint32_t newColor = newRed;
  newColor = (newColor << 8) | newGreen;
  newColor = (newColor << 8) | newBlue;
  newColor = (newColor << 8) | newWhite;

  for(int i=0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, newColor);
  }

  if (changed) {
    Log.Verbose("R=%d G=%d B=%d W=%d.",newRed,newGreen,newBlue,newWhite);
  }
}

