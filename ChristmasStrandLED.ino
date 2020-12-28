#include <Adafruit_NeoPixel.h>

// The 16.4 foot LED string has 300 lights + our ATtiny85 is connected to the end 
// acting like an additional WS2812, which makes the total 301.
// Pin 6 is connected to the LED string.
// The LED strip is GRB and runs at 800KHz (as does my code for the ATtiny85).
Adafruit_NeoPixel strip(301, 6, NEO_GRB + NEO_KHZ800);

// Settings for common colors
const uint32_t redColor = strip.Color(255, 0, 0);
const uint32_t greenColor = strip.Color(0, 255, 0);
const uint32_t blueColor = strip.Color(0, 0, 255);
const uint32_t orangeColor = strip.Color(127, 63, 0);
const uint32_t whiteColor = strip.Color(255, 255, 255);
const uint32_t offColor = strip.Color(0, 0, 0);

// This a the color palette to use.
uint32_t colors[4];

// Set colors to Red, Green, Blue and Orange.
void setPaletteRGBO()
{
  colors[0] = orangeColor;
  colors[1] = redColor;
  colors[2] = greenColor;
  colors[3] = blueColor;
}

void setup() {
  strip.begin();
  strip.show();

  // NeoPixel brightness must be set to 63, so that lower 6 bits are on when 
  // set the pixel to a 255 value.
  strip.setBrightness(63);

  setPaletteRGBO();
}

void loop() {
  // Breath a whitish pink (star on)
  effect2();
  
  // Blink start with fast easter color random (faster when lit)
  effect4();

  // Shimmering star, color palette of red,green,blue,orange. 
  effect5();

  // Rising Red, Blue, Green (start off when at top).
  effect6();

  // Blink start with fast RGB color random (faster when lit)
  effect7();

  // Fast blinking (Red, green, blue & orange) + star flicker.
  effect8ForeverLoop();
}

// Breath a whitish pink (star on)
void effect2()
{
  for (int i = 0; i < 2; i++) {
    rampAllLights();
  }
}

// Blink start with fast easter color random (faster when lit)
void effect4()
{
  int lightIndex = 0;
  starOn();

  for (int i = 0; i < 3; i++)
  {
    for (int lightIndex = 0; lightIndex <= 30; lightIndex++) {
      if (lightIndex >= 30) {
        starOn();
      }
      else if (lightIndex > 20) {
        starOff();
        strip.show();
        // Make the effect so the colors change slower when 
        // star is not on.
        delay(50);
      } 
      
      colorRand();
    }
  }
}

// Shimmering star, color palette of red,green,blue,orange. 
void effect5()
{
  for (int i = 0; i < 8; i++)
  {
    colorRandPalette();
    for (int i = 0; i < 3; i++) {
      twinkle();
    }
  }
}

// Rising Red, Blue, Green (start off when at top).
void effect6()
{
  for (int i = 0; i < 3; i++)
  {
    const int segmentLength = 2;
    const int waitTime = 3;
    const int delayMs = 100;
  
    for (int i = 0; i < sizeof(colors)/sizeof(colors[0]); i++) {
      fillColor(colors[i], segmentLength, waitTime);
      delay(delayMs);
    }
  }
}

// Blink start with fast RGB color random (faster when lit)
void effect7()
{
  int lightIndex = 0;
  for (int i = 0; i < 3; i++)
  {
    for (int lightIndex = 0; lightIndex <= 30; lightIndex++) {
      if (lightIndex >= 30) {
        starOn();
      }
      else if (lightIndex > 20) {
        starOff();
        strip.show();
        // Make the effect so the colors change slower when 
        // star is not on.
        delay(50);
      }
      
      colorRandPalette();
    }
  }
}

// Fast blinking (Red, green, blue & orange) + star flicker.
void effect8ForeverLoop()
{
  while (true)
  {
    if (random(100) < 80) {
      starOn();
    } else {
      starOff();
    }
  
    colorRandPalette();
  }
}

// Turn on the star connected to the end of the LED strip.
void starOn()
{
  strip.setPixelColor(strip.numPixels()-1, strip.Color(255,0,0));  
}

// Turn off the star connected to the end of the LED strip.
void starOff()
{
  strip.setPixelColor(strip.numPixels()-1, strip.Color(0,255,0));    
}

int c=10;
int cDir=1;
int cMax=25;
int cMin=10;
void twinkle()
{
  c+=cDir;
  if (c>cMax) {cDir=-1;} else if (c<=cMin) {cDir=1;}
  for (int i=0; i<5; i++) {
    starOn();
    strip.show();
    delay(c);
    starOff();
    strip.show();
    delay(c*2);  
  }
}

void colorRandPalette() {
  for(int i=0; i<strip.numPixels()-1; i++) {
    int rnd = random(100);
    if (rnd<20) {
      int c = random(sizeof(colors)/sizeof(colors[0]));
      strip.setPixelColor(i, colors[c]);    
    } else {
      strip.setPixelColor(i, 0);    
    }
    
    if (i==0 || i==200) {
      strip.show();
      starOn();
    }
  }
  
  strip.show();
}

void colorRand() {
  for(int i=0; i<strip.numPixels()-1; i++) {
    int rnd = random(100);
    if (rnd<25) {
      strip.setPixelColor(i, strip.Color(random(255),random(255),random(255)));    
    } else {
      strip.setPixelColor(i, offColor);    
    }
  }

  strip.show();
}

void rampAllLights() {
  const uint8_t wait = 5;
  
  for(int j=0; j<= 255; j+=2) {
    strip.fill(strip.Color(j/2, j, j));
    
    if (j%4 > 2) {
      starOn();
    }
    else {
      starOff();
    }

    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j-=2) {
    strip.fill(strip.Color(j/2, j, j));
    if (j%4>1) {
      starOn();
    }
    else {
      starOff();
    }
    strip.show();
    delay(wait);
  }
}

void fillColor(uint32_t color, int refreshLength, int refreshWaitTime) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);
    if (i%refreshLength == 0) {
      strip.show();
      delay(refreshWaitTime);
    }
  }

  strip.show();
}
