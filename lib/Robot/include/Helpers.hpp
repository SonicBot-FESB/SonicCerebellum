#include <MotorWithEncoder.hpp>
#include <DistanceSensor.hpp>

struct FrontSensorsPair {
  DistanceSensor *left;
  DistanceSensor *right;
} typedef FrontSensorsPair;

struct SideSensorsPair {
  DistanceSensor *front;
  DistanceSensor *back;
} typedef SideSensorsPair;

struct MotorsPair {
  Motor *front;
  MotorWithEncoder *back;

  void cw(uint8_t speed = 250) {
    this->front->cw(speed);
    this->back->cw(speed);
  }

  void ccw(uint8_t speed = 250) {
    this->front->ccw(speed);
    this->back->ccw(speed);
  }

  void stop() {
    this->front->stop();
    this->back->stop();
  }

} typedef MotorsPair;
