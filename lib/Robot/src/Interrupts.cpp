#include <Interrupts.hpp>
#include <PidControllers.hpp>
#include <Robot.hpp>

void countMotorLeft_ISR() {
  Robot &r = Robot::getInstance();
  r.countEncoder(LEFT_MOTOR);
}

void countMotorRight_ISR() {
  Robot &r = Robot::getInstance();
  r.countEncoder(RIGHT_MOTOR);
}

void calibrateForwardMovement_ISR() {
  StraightMovementPidController::getInstance().calibrate(MOVING_FORWARD);
}

void calibrateBackwardMovement_ISR() {
  StraightMovementPidController::getInstance().calibrate(MOVING_BACKWARDS);
}