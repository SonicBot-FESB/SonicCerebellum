#include <Arduino.h>
#include <Motor.hpp>

Motor::Motor(uint8_t pinCw, uint8_t pinCcw) {
  pinMode(pinCw, OUTPUT);
  pinMode(pinCcw, OUTPUT);

  this->pinCw = pinCw;
  this->pinCcw = pinCcw;
}

void Motor::cw(uint8_t speed) {
  analogWrite(this->pinCw, speed);
  analogWrite(this->pinCcw, 0);
}

void Motor::ccw(uint8_t speed) {
  analogWrite(this->pinCw, 0);
  analogWrite(this->pinCcw, speed);
}

void Motor::stop() {
  analogWrite(this->pinCw, 0);
  analogWrite(this->pinCcw, 0);
}