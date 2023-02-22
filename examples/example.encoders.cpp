#include <Arduino.h>
#include <consts.h>

volatile long unsigned int count_fwd = 0;
volatile long unsigned int count_bck = 0;

void encoder_left_ISR() {
  int M3_A_val = digitalRead(M3_A);
  count_fwd += 1 - M3_A_val;
  count_bck += M3_A_val;

  Serial.print(count_fwd);
  Serial.print("   ");
  Serial.println(count_bck);
}

void setup() {
  Serial.begin(9600);
  for (int pin : ENCODER_PINS) {
    pinMode(pin, INPUT_PULLDOWN);
  }
  for (int pin : MOTOR_PINS) {
    pinMode(pin, OUTPUT);
  }
  attachInterrupt(M3_B, encoder_left_ISR, FALLING);
}

void loop() {}