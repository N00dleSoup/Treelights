#include <FastLED.h>
#define NUM_LEDS 16
#define DATA_PIN 0
CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
}

void loop() {
  unsigned long timer = millis() % 5000;
  byte hue = timer / 20;
  FastLED.showColor(CHSV(hue, 255, 255));
}
