#include <Arduino.h>
#include <Motor.hpp>
#include <Types.hpp>

class MotorWithEncoder : public Motor {
private:
  volatile uint countCw = 0;
  volatile uint countCcw = 0;

  uint8_t encoderPinA;
  uint8_t encoderPinB;

public:
  int interruptPin;

  MotorWithEncoder(uint8_t pinCw, uint8_t pinCcw, uint8_t encoderPinA,
                   uint8_t encoderPinB);
  void resetEncoderCount();
  void encoderCount();
  void getEncoderCount(uint &cwCounter, uint &ccwCounter);
};