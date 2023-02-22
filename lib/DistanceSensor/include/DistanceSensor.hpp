#include <Arduino.h>

class DistanceSensor {
private:
  int pin;

public:
  DistanceSensor(uint8_t pin);
  ~DistanceSensor() { Serial.println("DESTROYED DISTANCE SENSOR"); }
  int read();
};