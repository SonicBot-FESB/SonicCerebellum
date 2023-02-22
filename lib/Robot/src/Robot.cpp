#include <Interrupts.hpp>
#include <PidControllers.hpp>
#include <Robot.hpp>
#include <Exceptions.hpp>

void Robot::init() {
  this->gyro = new GyroSensor(GYRO_SDA, GYRO_SCL, 55, 0x28, &Wire1);

  this->leftSensors = new SideSensorsPair;
  this->leftSensors->back = new DistanceSensor(PIN_IR_6);
  this->leftSensors->front = new DistanceSensor(PIN_IR_3);

  this->rightSensors = new SideSensorsPair;
  this->rightSensors->back = new DistanceSensor(PIN_IR_5);
  this->rightSensors->front = new DistanceSensor(PIN_IR_4);

  this->frontSensors = new FrontSensorsPair;
  this->frontSensors->left = new DistanceSensor(PIN_IR_2);
  this->frontSensors->right = new DistanceSensor(PIN_IR_1);

  this->leftMotors = new MotorsPair;
  this->leftMotors->back =
      new MotorWithEncoder(PIN_M4_CW, PIN_M4_CCW, PIN_M4_A, PIN_M4_B);
  this->leftMotors->front = new Motor(PIN_M2_CW, PIN_M2_CCW);

  this->rightMotors = new MotorsPair;
  this->rightMotors->back =
      new MotorWithEncoder(PIN_M3_CW, PIN_M3_CCW, PIN_M3_A, PIN_M3_B);
  this->rightMotors->front = new Motor(PIN_M1_CW, PIN_M1_CCW);
}

void Robot::initGyro() {
  if (!this->gyro->init()) {
    throw GyroFailedException();
  }
}

void Robot::attachEncoderCountInterrupts() {
  attachInterrupt(PIN_M4_B, countMotorLeft_ISR, FALLING);
  attachInterrupt(PIN_M3_B, countMotorRight_ISR, FALLING);
}

void Robot::detachEncoderCountInterrupts() {
  detachInterrupt(PIN_M4_B);
  detachInterrupt(PIN_M3_B);
}

void Robot::attachPidControlInterrupts(char pidControlGroupFlag) {
  switch (pidControlGroupFlag) {
  case PID_CONTROL_STRAIGHT:
    attachInterrupt(PIN_M3_A, calibrateForwardMovement_ISR, FALLING);
    break;
  case PID_CONTROL_BACKWARD:
    attachInterrupt(PIN_M3_A, calibrateBackwardMovement_ISR, FALLING);
  };
}

void Robot::detachPidControlInterrupts(char pidControlGroupFlag) {
  switch (pidControlGroupFlag) {
  case PID_CONTROL_STRAIGHT:
    detachInterrupt(PIN_M3_A);
    break;
  };
}

void Robot::forward() {
  this->stop();
  this->attachEncoderCountInterrupts();
  this->attachPidControlInterrupts(PID_CONTROL_STRAIGHT);

  this->leftMotors->cw(this->leftSideMotorSpeed);
  this->rightMotors->ccw(this->rightSideMotorSpeed);
  this->currentMovementDirectionFlag = MOVING_FORWARD;
}

void Robot::backward() {
  this->stop();
  this->attachEncoderCountInterrupts();
  this->attachPidControlInterrupts(PID_CONTROL_BACKWARD);

  this->leftMotors->ccw(this->leftSideMotorSpeed);
  this->rightMotors->cw(this->rightSideMotorSpeed);
  this->currentMovementDirectionFlag = MOVING_BACKWARDS;
}

void Robot::adjustMotorSpeed(uint8_t leftSideSpeed, uint8_t rightSideSpeed) {
  this->leftSideMotorSpeed = leftSideSpeed;
  this->rightSideMotorSpeed = rightSideSpeed;

  switch (this->currentMovementDirectionFlag) {
  case MOVING_FORWARD:
    this->leftMotors->cw(leftSideSpeed);
    this->rightMotors->ccw(rightSideSpeed);
    break;
  case MOVING_BACKWARDS:
    this->leftMotors->ccw(leftSideSpeed);
    this->rightMotors->cw(rightSideSpeed);
    break;
  };
}

void Robot::stop() {
  this->leftMotors->stop();
  this->rightMotors->stop();
  this->detachEncoderCountInterrupts();
  this->detachPidControlInterrupts(PID_CONTROL_STRAIGHT);
  this->currentMovementDirectionFlag = NOT_MOVING;
}

void Robot::turn(float degrees, bool resetPosition) {
  if (!resetPosition) {
    this->initialPosition = this->getPosition(AXIS_X);
    StraightMovementPidController &ctrl = StraightMovementPidController::getInstance();
    ctrl.setTargetDisplacement(degrees);
    return;
  }

  this->stop();
  char movementFlag = degrees > 0 ? TURNING_RIGHT : TURNING_LEFT;
  this->currentMovementDirectionFlag = movementFlag;

  this->initialPosition = this->getPosition(AXIS_X);

  TurningPidController &controller = TurningPidController::getInstance();
  controller.setTargetDisplacement(degrees);

  do {
    controller.calibrate(movementFlag);
  } while (!controller.isCalibrated());

  this->stop();

  this->initialPosition = this->getPosition(AXIS_X);
}

void Robot::adjustRotationVector(int rotationVector) {
  int speed = rotationVector < 0 ? -rotationVector : rotationVector;
  speed *= 0.5;

  if (speed < 48)
    speed = 48;

  if (rotationVector > 0) {
    this->leftMotors->cw(speed);
    this->rightMotors->cw(speed);
  } else if (rotationVector < 0) {
    this->leftMotors->ccw(speed);
    this->rightMotors->ccw(speed);
  }
}

float Robot::getRotation(char axis) {
  return this->gyro->readRotation(axis, this->initialPosition);
}

float Robot::getPosition(char axis) { return this->gyro->read(axis); }

void Robot::getEncodersCount(uint &leftMotorCwCount, uint &leftMotorCcwCount,
                             uint &rightMotorCwCount,
                             uint &rightMotorCcwCount) {
  this->leftMotors->back->getEncoderCount(leftMotorCwCount, leftMotorCcwCount);
  this->rightMotors->back->getEncoderCount(rightMotorCwCount,
                                           rightMotorCcwCount);
}

void Robot::resetEncoderCount(char motorFlag) {
  switch (motorFlag) {
  case LEFT_MOTOR:
    this->leftMotors->back->resetEncoderCount();
    break;
  case RIGHT_MOTOR:
    this->rightMotors->back->resetEncoderCount();
    break;
  };
}

void Robot::resetEncoderCount() {
  this->leftMotors->back->resetEncoderCount();
  this->rightMotors->back->resetEncoderCount();
}

void Robot::countEncoder(char motorFlag) {
  switch (motorFlag) {
  case LEFT_MOTOR:
    this->leftMotors->back->encoderCount();
    break;
  case RIGHT_MOTOR:
    this->rightMotors->back->encoderCount();
    break;
  }
}

void Robot::getMotorSpeed(uint8_t &leftMotorsSpeed, uint8_t &rightMotorsSpeed) {
  leftMotorsSpeed = this->leftSideMotorSpeed;
  rightMotorsSpeed = this->rightSideMotorSpeed;
}

void Robot::getDistance(uint16_t &frontLeft, uint16_t &frontRight,
                        uint16_t &leftFront, uint16_t &leftBack,
                        uint16_t &rightFront, uint16_t &rightBack) {
  frontLeft = this->frontSensors->left->read();
  frontRight = this->frontSensors->right->read();

  leftFront = this->leftSensors->front->read();
  leftBack = this->leftSensors->back->read();

  rightFront = this->rightSensors->front->read();
  rightBack = this->rightSensors->back->read();
}

void Robot::resetInitialPosition() {
  this->initialPosition = this->getPosition(AXIS_X);
}
