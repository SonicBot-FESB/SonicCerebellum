#include <Arduino.h>
#include <Helpers.hpp>
#include <Pins.hpp>
#include <Types.hpp>
#include <GyroSensor.hpp>

#define LEFT_MOTOR 'L'
#define RIGHT_MOTOR 'R'

#define X_AXIS 'X'
#define Y_AXIS 'Y'
#define Z_AXIS 'Z'

#define NOT_MOVING 0
#define MOVING_FORWARD 1
#define MOVING_BACKWARDS 2
#define TURNING_RIGHT 3
#define TURNING_LEFT 4

#define PID_CONTROL_STRAIGHT 0
#define PID_CONTROL_BACKWARD 1

class Robot {
public:
  static Robot &getInstance() {
    static Robot instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    return instance;
  }

private:
  Robot(){};
  ~Robot(){};

private:
  SideSensorsPair *leftSensors = nullptr;
  SideSensorsPair *rightSensors = nullptr;
  FrontSensorsPair *frontSensors = nullptr;
  GyroSensor *gyro = nullptr;

  MotorsPair *leftMotors = nullptr;
  MotorsPair *rightMotors = nullptr;

  float initialPosition = 0;
  char currentMovementDirectionFlag = NOT_MOVING;

  uint8_t leftSideMotorSpeed = DEFAULT_SPEED;
  uint8_t rightSideMotorSpeed = DEFAULT_SPEED;

public:
  void init();
  void initGyro();
  void attachEncoderCountInterrupts();
  void detachEncoderCountInterrupts();
  void attachPidControlInterrupts(char pidControlGroupFlag);
  void detachPidControlInterrupts(char pidControlGroupFlag);
  void forward();
  void backward();
  void adjustMotorSpeed(uint8_t leftSideSpeed, uint8_t rightSideSpeed);
  void stop();
  void turn(float degrees, bool resetPosition);
  void adjustRotationVector(int rotationVector);
  float getRotation(char axis);
  float getPosition(char axis);
  void getEncodersCount(uint &leftMotorCwCount, uint &leftMotorCcwCount,
                        uint &rightMotorCwCount, uint &rightMotorCcwCount);
  void resetEncoderCount(char motorFlag);
  void resetEncoderCount();
  void countEncoder(char motorFlag);
  void getMotorSpeed(uint8_t &leftMotorsSpeed, uint8_t &rightMotorsSpeed);
  void getDistance(uint16_t &frontLeft, uint16_t &frontRight,
                   uint16_t &leftFront, uint16_t &leftBack,
                   uint16_t &rightFront, uint16_t &rightBack);
  void resetInitialPosition();
};
