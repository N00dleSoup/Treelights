#include <avr/interrupt.h>
volatile bool val = true;

void setup() {
  delay(2000);
  pinMode(1, OUTPUT);
  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00001000;    // turn on interrupts on pins PB3
  sei();                 // enables interrupts

}

void loop() {
  digitalWrite(1, val);
  
}


ISR(PCINT0_vect) {
  val = !val;
}

