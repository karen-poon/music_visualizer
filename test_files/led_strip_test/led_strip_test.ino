#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

void setup() { 
      Serial.begin(9600);
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      FastLED.setBrightness(60);
}

void loop() { 
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(50);
  }
}
