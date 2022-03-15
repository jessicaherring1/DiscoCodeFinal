//POPART PCB DISCO BALL

//buttons
#include <Bounce2.h>
#define BOUNCE_PIN 1
boolean isButton = false;
Bounce bounce = Bounce();

//neopixels
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(27, 0);

//frames
int numGrid = 27;
int numFrames = 2;
int j = 0;

char index[27] {
  0,
  1, 2, 3,
  4, 5,  6,  7,
  8, 9, 10, 11,
  12, 13, 14, 15,
  16, 17, 18, 19,
  20, 21, 22, 23,
  24, 25, 26,
};

char colors[3][3] {
  {0, 0, 255}, //blue
  {255, 0, 0}, //red
  {0, 0, 0}, //off
};

char frames[2][27] {
  {
    // commented out frames represent all neopixels switching colors every frame 
    //    1,
    //    1, 0, 1,
    //    0, 1, 0, 1,
    //    1, 0, 1, 0,
    //    0, 1, 0, 1,
    //    1, 0, 1, 0,
    //    0, 1, 0, 1,
    //    1, 0, 1
    //  },
    //  {
    //    0,
    //    0, 1, 0,
    //    1, 0, 1, 0,
    //    0, 1, 0, 1,
    //    1, 0, 1, 0,
    //    0, 1, 0, 1,
    //    1, 0, 1, 0,
    //    0, 1, 0
    //  },

    // new frames represent blocks of 3-5 neopixels changing every frame. This makes the movement more apparent through the diffusion screen
    1,
    1, 1, 0,
    1, 1, 0, 0,
    0, 0, 1, 1,
    0, 0, 1, 1,
    1, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 1
  },
  {
    0,
    0, 0, 1,
    0, 0, 1, 1,
    1, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 1, 1,
    0, 0, 1, 1,
    1, 1, 0
  },

};

//programatic function variables
int i;
int r;
int g;
int b;

//extra function variables
boolean isOn;
boolean start;

//strandTest function variables
int color = 0; //0-255
long colorTimer = 0;
long colorInterval = 10;

//timers
unsigned long startTime = millis();
unsigned long endTime;
long interval = 300;
long interval2 = 50;
long interval3 = 200;

//switch case
int state = 1;


void setup() {
  // strip
  strip.begin();
  strip.clear();
  strip.show();

  // button
  bounce.attach( BOUNCE_PIN, INPUT );
  bounce.interval(5);
}

void loop() {
  button();

  switch (state) {
    case 0:
      // off state
      break;

    case 1:
      framesFunc();
      break;

    case 2:
      programmatic();
      break;

    case 3:
      extra();
      break;

    case 4:
      strandTest();
      break;
  }
}


// checks to see if the button has been pressed, and if so, changes the state of the program
void button() {
  bounce.update();

  if (bounce.changed()) {
    int debouncedInput = bounce.read();

    if (debouncedInput == HIGH) {
      strip.clear();
      strip.show();
      state = (state + 1) % 5;
    }
  }
}


// this is a animation is created through the use of two pre-defined frames
  // it uses the frames to create the appearance of a spinning ball
void framesFunc() {
  endTime = millis();
  if (endTime - startTime >= interval) {
    if (j < numFrames - 1) {
      j++;
    } else {
      j = 0;
    }
    startTime = millis();
  }
  int whichFrame = j;
  for (int i = 0; i < numGrid; i++) {
    int whichLED  = i;
    int whichIndex = index[whichLED];
    int whichColor = frames[whichFrame][whichLED];

    int r = colors[whichColor][0];
    int g = colors[whichColor][1];
    int b = colors[whichColor][2];
    strip.setPixelColor(whichIndex, r, g, b);
  }
  strip.show();
}


//depicts a soft movement of colors by setting adjacent sets of neopixels to 
void programmatic() {
  endTime = millis();
  //chooses a new neopixel index and color value every interval
  if (endTime - startTime >= interval2) {
    i = random(27);
    r = random(155);
    g = random(155);
    b = random(155);
    startTime = millis();
  }
  //sets two adjacent neopixels to the same color
  strip.setPixelColor(i, r, g, b);
  strip.setPixelColor(i + 1, r, g, b);
  strip.show();
}


// represents light reflecting off of a disco ball
  // all neopixels are blue and every interval, a random neopixel is set to a random color, and then reset to blue
void extra() {
  //turns all of the neopixels blue only once at the very beggining
  while (start) {
    for (int i = 0; i < 27; i++) {
      //strip.setBrightness(25);
      strip.setPixelColor(i, 0, 0, 100); //not a very bright blue
      strip.show();
    }
    start = false;
  }

  endTime = millis();
  //updates the isOn boolean every interval
  if (endTime - startTime >= interval3) {
    isOn = !isOn;
    startTime = millis();
  }
  int i = random(27);
  r = random(255);
  g = random(255);
  b = random(255);
  //sets the pixel to a random color every interval as the boolean is set to true
  if (isOn) {

    strip.setPixelColor(i, r, g, b);
    strip.show();
    isOn = false;
  }
  //resets the same pixel back to blue
  else {
    strip.setPixelColor(i, 0, 0, 100);
  }
}


//cycles between colors at a slow rate causing a gradient effect 
  // this is not my code! 
  // credit: Zane Cochran
void strandTest() {
  if (millis() - colorTimer > colorInterval) {
    color = (color + 1) % 255;
    colorTimer = millis();
  }

  for (int i = 0; i < 27; i++) {
    strip.setPixelColor(i, Wheel(color));
  }
  strip.show();
}


//helper function for the strandTest function above
  // this is also not my code!
  // credit: Zane Cochran
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
