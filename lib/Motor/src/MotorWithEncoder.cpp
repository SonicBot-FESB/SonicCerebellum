#include <MotorWithEncoder.hpp>

MotorWithEncoder::MotorWithEncoder(uint8_t pinCw, uint8_t pinCcw,
                                   uint8_t encoderPinA, uint8_t encoderPinB)
    : Motor(pinCw, pinCcw) {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  this->encoderPinA = encoderPinA;
  this->encoderPinB = encoderPinB;
  this->interruptPin = encoderPinB;
}

// Attach interrupt to pin B when FALLING
void MotorWithEncoder::encoderCount() {
  int encoderPointA = digitalRead(this->encoderPinA);
  this->countCcw += 1 - encoderPointA;
  this->countCw += encoderPointA;
}

void MotorWithEncoder::getEncoderCount(uint &cwCounter, uint &ccwCounter) {

  cwCounter = this->countCw;
  ccwCounter = this->countCcw;
}

void MotorWithEncoder::resetEncoderCount() {
  this->countCw = 0;
  this->countCcw = 0;
}
