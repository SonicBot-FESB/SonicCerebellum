#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

#define AXIS_X 'X'
#define AXIS_Y 'Y'
#define AXIS_Z 'Z'

class GyroSensor : private Adafruit_BNO055 {
public:
  GyroSensor(uint8_t SDA, uint8_t SCL, int32_t sensorID = -1,
             uint8_t address = BNO055_ADDRESS_A, TwoWire *theWire = &Wire);
  ~GyroSensor() { Serial.println("DESTROYED GYRO SENSOR"); }
  bool init();
  float read(char axis);
  float readRotation(char axis, float startPoint);
};