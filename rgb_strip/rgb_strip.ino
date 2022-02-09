#include <FastLED.h>

#define NUM_LEDS 300      // The amount of leds you have
#define DATA_PIN 9       // LED pin on Arduino
#define LED_TYPE WS2812B // LED type
#define SATURATION 255   // saturation of leds
int brightness = 120;   // brightness of leds, 0-255
const int WAVE_BUTTON = 2; // each light setting corresponds to a different button on breadboard
const int STACK_BUTTON = 3;
const int WHITE_BUTTON = 4;
const int BREATHE_BUTTON = 5;
const int c = 6; // placeholders for extra buttons
const int b = 7;
const int POWER_OFF_BUTTON = 8; // button used to clear lights
int mode = 0;  // tells arduino light setting is currently activated
const int MODE_OFF = 0; // each mode
const int MODE_WAVE = 1;
const int MODE_STACK = 2;
const int MODE_WHITE = 3;
const int MODE_BREATHE = 4;
void powerOff(); // clears LEDs
void white(); // pure white light (may lose color if too many leds and not enough voltage)
void wave(); // rainbow wave
void breathe(); // breathe (lights fade in and out)
void stack(); // stack (lights stack from one side then de-stack)
bool powerJustPressed();

CRGB leds[NUM_LEDS]; // array of leds

void setup() {
//  Serial.begin(9600); // used for debugging
  pinMode(POWER_OFF_BUTTON, INPUT_PULLUP); // pinmode for each button to take input
  pinMode(WAVE_BUTTON, INPUT_PULLUP);
  pinMode(STACK_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  pinMode(BREATHE_BUTTON, INPUT_PULLUP);
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);  // add leds depending on settings above
}

void loop() {
  if(digitalRead(POWER_OFF_BUTTON) == LOW)    // checks if any button is pressed
  {
    mode = MODE_OFF;
  }
  if(digitalRead(WAVE_BUTTON) == LOW)
  {
    mode = MODE_WAVE;
  }
  if(digitalRead(STACK_BUTTON) == LOW)
  {
    mode = MODE_STACK;
  }
  if(digitalRead(WHITE_BUTTON) == LOW)
  {
    mode = MODE_WHITE;
  }
  if(digitalRead(BREATHE_BUTTON) == LOW)
  {
    mode = MODE_BREATHE;
  }
  switch(mode)        // switch case that calls corresponding light setting functions
  {
    case MODE_OFF:
      powerOff();
      break;
    case MODE_WAVE:
      wave();
      break;
     case MODE_STACK:
      stack();
      break;
    case MODE_WHITE:
      white();
      break;
    case MODE_BREATHE:
      breathe();
      break;
  }
  FastLED.show();     // needed for leds to light
}

void wave()   // rainbow wave
{
  brightness = 140;
  int j = (millis()/30);      // change "30" to change speed of led setting
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(i - (j * 2), SATURATION, brightness);
  }
}

void white()      // function used to make all leds pure white and provide light
{
  brightness = 255;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(brightness, brightness, brightness);
  }
}

void stack()    // stacks lights, then destacks
{
  brightness = 160;
  CRGB color = CRGB::SpringGreen;   // change color by replacing "SpringGreen" with any supported FastLED color
  color.nscale8(brightness);
  int count = ((millis()/20) % 600);    // can change speed in same way as wave() function
  int numLit = count;               // change 300 and 600 to your number of leds and double that amount, respectively
  if(count > 300)
  {
    numLit = 600 - count;
  }
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(i < numLit)
    {
      leds[i] = color;
    }
    else
    {
      leds[i] = CRGB::Black;
    }
  }
}

void breathe()
{
  CRGB color = CRGB::Fuchsia;   // change color by replacing "Fuchsia" with any supported FastLED color
  int count = ((millis()/10) % 512);  // can change speed in same way as wave() function
  int breatheBrightness = count;
  if(count >= 256)
  {
    breatheBrightness = 512 - count - 1;
  }
//  Serial.println(breatheBrightness);    // used for debugging
//  delay(25);
  color.nscale8(breatheBrightness);
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}

void powerOff() // clears leds
{
  FastLED.clear();
}
