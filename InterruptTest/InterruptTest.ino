#define FASTLED_ALLOW_INTERRUPTS 1
#include <FastLED.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


#define NUM_LEDS 16
#define DATA_PIN 0
CRGB leds[NUM_LEDS];
volatile bool val = false;

void setup() {
  delay(2000);
  pinMode(1, INPUT_PULLUP);
  sbi(GIMSK,PCIE); // Turn on Pin Change interrupt
  sbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
}

void loop() {
  if(val) {
    leds[3] = CRGB::Purple;
    FastLED.show();
  }
  
}


ISR(PCINT0_vect) {
  val = true;
}

