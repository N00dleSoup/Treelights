#include <FastLED.h>

#include <avr/interrupt.h>
#define NUM_LEDS 16
#define DATA_PIN 0
#define POT_PIN A1
#define BUTTON_PIN 1
#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160
#define NUM_PATTERNS 3
CRGB leds[NUM_LEDS];
volatile int pattern = 1;
unsigned long sensorDelay = 40;
unsigned long lastSensor = 0;
bool lastButtonState = HIGH;


void redGreen() {
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

void fallingLights(){
  int state = 3-(millis()%2000)/500;
  for (int i = 0; i<4; i++){
    leds[i*4+(state+i)%4] = CRGB::White;
  }
  fadeToBlackBy(leds,16,10);
}

//Sweep around the HSV spectrum once every 5 seconds
void hueSweep() {
  unsigned long timer = millis() % 5000;
  byte hue = timer / 20;
  CRGB currColor = CHSV(hue, 255,200);
  for(int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = currColor;
  }
}

void setup() {
  delay(2000);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
/* //Interrupts don't work for some reason????
  GIMSK = 0b00100000;
  PCMSK = 0b00000010;
//  _SFR_BYTE(GIMSK) &= ~_BV(PCIE);
//  _SFR_BYTE(PCMSK) &= ~_BV(PCINT1);
  sei();
*/
}
  
void loop() {
  if (pattern == 0) {
    fallingLights();
  }
  else if (pattern == 1) {
    redGreen();
  }
  else if (pattern == 2) {
    hueSweep();
  }
  FastLED.show();

  //Set the pattern based on the pot value
  unsigned long now = millis();
  if (now >= lastSensor + sensorDelay) {
    lastSensor = now;
    int reading = analogRead(POT_PIN);
    //Padding between the patterns so it doesn't flicker
    int padding = 40;
    int patternRange = (1023 - padding*(NUM_PATTERNS-1)) / NUM_PATTERNS;
    for(int i = 0; i < NUM_PATTERNS; ++i) {
      if((i*(patternRange+padding) <= reading) && (reading < (i+1)*patternRange + i*padding)) {
        pattern = i;
        break;
      }
    }
  }

  
  //Every so often, set brightness according to value of potentiometer
//  unsigned long now = millis();
//  if(now >= lastSensor + sensorDelay) {
//    lastSensor = now;
//    int reading = analogRead(POT_PIN);
//    FastLED.setBrightness(map(reading, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
//  }
//  //WHY DOESN'T THIS WORK???
//  bool buttonState = digitalRead(BUTTON_PIN);
//  if(buttonState != lastButtonState && buttonState == HIGH) {
//    ++pattern;
//    pattern = pattern % 2;
//  }
//  lastButtonState = buttonState;
}


/*
ISR(PCINT0_vect){
//  pattern++;
//  pattern = pattern %2;
  leds[0] = CRGB::Red;
  FastLED.show();
} 
*/
