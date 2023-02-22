#include <GyroSensor.hpp>

GyroSensor::GyroSensor(uint8_t SDA, uint8_t SCL, int32_t sensorID,
                       uint8_t address, TwoWire *theWire)
    : Adafruit_BNO055(sensorID, address, theWire) {
  theWire->setSDA(SDA);
  theWire->setSCL(SCL);
}

bool GyroSensor::init() { return this->begin(); }

float GyroSensor::read(char axis) {
  sensors_event_t event;
  this->getEvent(&event);

  switch (axis) {
  case AXIS_Y:
    return event.orientation.x;
  case AXIS_Z:
    return event.orientation.z;
  default: // AXIS_X
    return event.orientation.x;
  }
}

float GyroSensor::readRotation(char axis, float offset) {
  float value = this->read(axis);
  value -= offset;

  if (value > 360)
    value = value - 360;
  else if (value < 0)
    value = value + 360;

  if (value > 180) {
    value -= 360;
  }

  return value;
}
