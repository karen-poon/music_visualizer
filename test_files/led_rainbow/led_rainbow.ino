#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 3

CRGB leds[NUM_LEDS];
float red, green, blue;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(60);
}

void loop() {
  for (float t = 0; t < 1000; t = t + 0.07) {
    red = 126 + 126 * sin(2 * 3.141592654 * 0.03 * t);
    green = 64 + 64 * sin(2 * 3.141592654 * 0.027 * t);
    blue = 32 + 32 * sin(2 * 3.141592654 * 0.025 * t);
    FastLED.showColor(CRGB(red, green, blue));
    delay(10);
  }
}


