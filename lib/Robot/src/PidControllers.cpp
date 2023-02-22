#include <Arduino.h>
#include <PidControllers.hpp>
#include <Robot.hpp>

MovementPidController::MovementPidController(int Kp, int Ki, int Kd) {
  this->targetDisplacement = 0;
  this->strafeRight =
      new PID(&this->currentDisplacement, &this->strafeRightOutput,
              &this->targetDisplacement, Kp, Ki, Kd, REVERSE);

  this->strafeLeft =
      new PID(&this->currentDisplacement, &this->strafeLeftOutput,
              &this->targetDisplacement, Kp, Ki, Kd, DIRECT);

  this->strafeRight->SetMode(AUTOMATIC);
  this->strafeLeft->SetMode(AUTOMATIC);
}

StraightMovementPidController *StraightMovementPidController::instance =
    nullptr;

StraightMovementPidController &StraightMovementPidController::getInstance() {
  if (!instance)
    instance = new StraightMovementPidController();
  return *instance;
}

StraightMovementPidController::StraightMovementPidController()
    : MovementPidController(STRAIGHT_MOVEMENT_PID_KP, STRAIGHT_MOVEMENT_PID_KI,
                            STRAIGHT_MOVEMENT_PID_KD) {}

void StraightMovementPidController::setTargetDisplacement(
    float targetDisplacement) {
  this->targetDisplacement = targetDisplacement;
}

void StraightMovementPidController::calibrate(char directionFlag) {
  Robot &r = Robot::getInstance();

  this->currentDisplacement = r.getRotation(AXIS_X);
  strafeLeft->Compute();
  strafeRight->Compute();

  switch (directionFlag) {
  case MOVING_FORWARD:
    r.adjustMotorSpeed(DEFAULT_SPEED - (strafeRightOutput * 0.25),
                       DEFAULT_SPEED - (strafeLeftOutput * 0.25));
    break;
  case MOVING_BACKWARDS:
    r.adjustMotorSpeed(DEFAULT_SPEED - (strafeLeftOutput * 0.25),
                       DEFAULT_SPEED - (strafeRightOutput * 0.25));
  };
}

TurningPidController::TurningPidController()
    : MovementPidController(ROTATING_PID_KP, ROTATING_PID_KI, ROTATING_PID_KD) {
}

TurningPidController *TurningPidController::instance = nullptr;

TurningPidController &TurningPidController::getInstance() {
  if (!instance)
    instance = new TurningPidController();
  return *instance;
}

void TurningPidController::setTargetDisplacement(float targetDisplacement) {
  this->targetDisplacement = targetDisplacement;
  for (float &val : measurements) {
    val = 0;
  }
}

void TurningPidController::calibrate(char directionFlag) {
  Robot &r = Robot::getInstance();
  this->currentDisplacement = r.getRotation(AXIS_X);

  this->pushMeasurement(this->currentDisplacement);

  this->strafeLeft->Compute();
  this->strafeRight->Compute();

  int rotationVector = (strafeLeftOutput - strafeRightOutput);

  r.adjustRotationVector(rotationVector);
}

void TurningPidController::pushMeasurement(float value) {
  if (lastMeasurementIndex != (MEASUREMENTS_SAMPLE - 1)) {
    measurements[lastMeasurementIndex] = value;
    lastMeasurementIndex += 1;
  }
  lShiftMeasurements();
  measurements[lastMeasurementIndex] = value;
}

void TurningPidController::lShiftMeasurements() {
  for (uint8_t i = 1; i < MEASUREMENTS_SAMPLE; i++) {
    measurements[i - 1] = measurements[i];
  }
}

bool TurningPidController::isCalibrated() {
  float lowerLimit = targetDisplacement - 0.5;
  float upperLimit = targetDisplacement + 0.5;

  for (float &val : measurements) {
    if (!(val < upperLimit && val > lowerLimit))
      return false;
  }
  return true;
}
