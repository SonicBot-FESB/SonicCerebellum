#include <DistanceSensor.hpp>

DistanceSensor::DistanceSensor(uint8_t pin) {
  pinMode(pin, INPUT);
  this->pin = pin;
}

int DistanceSensor::read() { return analogRead(this->pin); }