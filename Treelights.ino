#include <FastLED.h>

#include <avr/interrupt.h>
#define NUM_LEDS 16
#define DATA_PIN 0
#define POT_PIN A1
#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160
CRGB leds[NUM_LEDS];
volatile int pattern = 1;
unsigned long sensorDelay = 40;
unsigned long lastSensor = 0;


void RedGreen() {
  if (millis() % 1000 > 500) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2)
        leds[i] = CRGB::Red;
      else
        leds[i] = CRGB::Green;
    }
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2)
        leds[i] = CRGB::Green;
      else
        leds[i] = CRGB::Red;
    }
  }
}

void FallingLights(){
  int state = 3-(millis()%2000)/500;
  for (int i = 0; i<4; i++){
    leds[i*4+(state+i)%4] = CRGB::White;
  }
  fadeToBlackBy(leds,16,10);
}


void setup() {
  delay(2000);
  pinMode(1,INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
/* //FastLED disables interrupts while it's outputting data :(
  GIMSK = 0b00100000;
  PCMSK = 0b00000010;
//  _SFR_BYTE(GIMSK) &= ~_BV(PCIE);
//  _SFR_BYTE(PCMSK) &= ~_BV(PCINT1);
  sei();
*/
}
  
/* Normal operation w/ pattern selection */
void loop() {
  if (pattern == 0) {
    FallingLights();
  }
  else {
    RedGreen();
  }
  FastLED.show();
  //Every so often, set brightness according to value of potentiometer
  unsigned long now = millis();
  if(now >= lastSensor + sensorDelay) {
    lastSensor = now;
    int reading = analogRead(POT_PIN);
    FastLED.setBrightness(map(reading, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
  }
}

/*
ISR(PCINT0_vect){
//  pattern++;
//  pattern = pattern %2;
  leds[0] = CRGB::Red;
  FastLED.show();
} 
*/
