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
const int LIGHTNING_BUTTON = 6;
const int BRIGHTNESS_BUTTON = 7;
const int POWER_OFF_BUTTON = 8; // button used to clear lights
int mode = 0;  // tells arduino light setting is currently activated
const int MODE_OFF = 0; // each mode
const int MODE_WAVE = 1;
const int MODE_STACK = 2;
const int MODE_WHITE = 3;
const int MODE_BREATHE = 4;
const int MODE_LIGHTNING = 5;
void powerOff(); // clears LEDs
void white(); // pure white light (may lose color if too many leds and not enough voltage)
void wave(); // rainbow wave
void breathe(); // breathe (lights fade in and out)
void stack(); // stack (lights stack from one side then de-stack)
void lightning();
bool powerJustPressed();

CRGB leds[NUM_LEDS]; // array of leds

void setup() {
//  Serial.begin(9600); // used for debugging
  pinMode(POWER_OFF_BUTTON, INPUT_PULLUP); // pinmode for each button to take input
  pinMode(WAVE_BUTTON, INPUT_PULLUP);
  pinMode(STACK_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  pinMode(BREATHE_BUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON, INPUT_PULLUP);
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
  if(digitalRead(LIGHTNING_BUTTON) == LOW)
  {
    mode = MODE_LIGHTNING;
  }
  if(digitalRead(BRIGHTNESS_BUTTON) == LOW)
  {
    brightness += 50;
    if(brightness >255)
    {
      brightness = 5;
    }
  }
  switch(mode)     // switch case that calls corresponding light setting functions
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
    case MODE_LIGHTNING:
      lightning();
      break;
  }
  FastLED.show();     // needed for leds to light
}

void wave()   // rainbow wave
{
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

void lightning()
{
  uint8_t speedy = 192;                                         // Low value = greater delay, up to 255 for no delay.
  uint8_t intensity = 192;
  uint16_t ledstart = random16(NUM_LEDS);                     // Determine starting location of flash.
  uint16_t ledlen = 1 + random16(NUM_LEDS - ledstart);        // Determine length of flash.
  uint8_t bri = 255/random8(1, 3);                            // Brightness.
  static long step;
  
  static uint16_t segDelay;                                   // Delay between strikes (was SEGMENT.aux0).
  static uint16_t numFlash = 0;                               // Flash stepper (was SEGMENT.aux1).
  
  if (numFlash == 0)                                          // Initialize the leader flash.
  {
    numFlash = random8(4, 4 + intensity/20);                  // Number of flashes.
    numFlash *= 2;
    bri = 52;                                                 // Leader has lower brightness.
    segDelay = 200;                                           // 200ms delay after leader.
  }
 
  fill_solid(leds,NUM_LEDS,0);                                // Fill the led's with black
  
  if (numFlash > 3 && !(numFlash & 0x01))                     // Flash on even number > 2.
  {                       
    for (int i = ledstart; i < ledstart + ledlen; i++)
    {
      leds[i] = CRGB::Gray;                                   // Fill ledstart to ledstart+ledlen with gray.
    }
    numFlash--;
    step = millis();
  } else {
    if (millis() - step > segDelay)
    {
      numFlash--;
      if (numFlash < 2) numFlash = 0;
      segDelay = (50 + random8(100));                               // Delay between flashes.
      if (numFlash == 2) segDelay = (random8(255 - speedy) * 100);  // Delay between strikes.
      step = millis();
    }
  }
}


void powerOff() // clears leds
{
  FastLED.clear();
}
