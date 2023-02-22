#include <Arduino.h>

#define DEFAULT_SPEED 130

class Motor {
private:
  uint8_t pinCw;
  uint8_t pinCcw;

public:
  Motor(uint8_t pinCw, uint8_t pinCcw);
  ~Motor() { Serial.println("DESTROYED MOTOR"); }
  void cw(uint8_t speed = DEFAULT_SPEED);
  void ccw(uint8_t speed = DEFAULT_SPEED);
  void stop();
};
