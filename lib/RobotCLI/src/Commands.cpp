#include <Arduino.h>
#include <Commands.hpp>
#include <Robot.hpp>
#include <SimpleCLI.h>

const void FWD::init(const Command &cmd) {}

void FWD::callback(cmd *cmdPtr) {
  Robot::getInstance().forward();
  Serial.println("FWD");
}

const void BCK::init(const Command &cmd) {}

void BCK::callback(cmd *cmdPtr) {
  Robot::getInstance().backward();
  Serial.println("BCK");
}

const void TRN::init(const Command &cmd) {
  char defaultTurnDirection = TURN_RIGHT;
  cmd.addPositionalArgument("degrees", "90");
  cmd.addPositionalArgument("direction", &defaultTurnDirection);
  cmd.addPositionalArgument("resetPosition", "1");
}

void TRN::callback(cmd *cmdPtr) {
  Command cmd(cmdPtr);
  Argument stepsArg = cmd.getArgument("degrees");
  Argument directionArg = cmd.getArgument("direction");
  Argument resetPositionArg = cmd.getArgument("resetPosition");

  int turnAmount = stepsArg.getValue().toInt();
  String turnDirection = directionArg.getValue();
  int resetPosition = resetPositionArg.getValue().toInt();

  if (turnDirection[0] == TURN_LEFT) {
    turnAmount *= -1;
  }

  Robot &r = Robot::getInstance();
  r.turn(turnAmount, resetPosition == 1);

  float position = r.getPosition(AXIS_X);
  float rotation = r.getRotation(AXIS_X);

  Serial.print("TRN ");
  Serial.print(position); // Absolute
  Serial.print(" ");
  Serial.println(rotation); // Relative
}

const void POS::init(const Command &cmd) {}

void POS::callback(cmd *cmdPtr) {
  Robot &r = Robot::getInstance();
  float position = r.getPosition(X_AXIS);
  float rotation = r.getRotation(X_AXIS);

  Serial.print("POS ");
  Serial.print(position); // Absolute
  Serial.print(" ");
  Serial.println(rotation); // Relative
}

const void ENC::init(const Command &cmd) {}

void ENC::callback(cmd *cmdPtr) {
  uint leftMotorEncoderCw, leftMotorEncoderCcw, rightMotorEncoderCw,
      rightMotorEncoderCcw;
  Robot::getInstance().getEncodersCount(leftMotorEncoderCw, leftMotorEncoderCcw,
                                        rightMotorEncoderCw,
                                        rightMotorEncoderCcw);

  Robot::getInstance().resetEncoderCount(LEFT_MOTOR);
  Robot::getInstance().resetEncoderCount(RIGHT_MOTOR);

  Serial.print("ENC ");
  Serial.print(leftMotorEncoderCw);
  Serial.print(" ");
  Serial.print(leftMotorEncoderCcw);
  Serial.print(" ");
  Serial.print(rightMotorEncoderCw);
  Serial.print(" ");
  Serial.println(rightMotorEncoderCcw);
}

const void STP::init(const Command &cmd) {}

void STP::callback(cmd *cmdPtr) {
  Robot::getInstance().stop();
  Serial.println("STP");
}

const void RBT::init(const Command &cmd) {}

void RBT::callback(cmd *cmdPtr) {
  Serial.println("RBT");
  _reboot_Teensyduino_();
}

const void RPS::init(const Command &cmd) {}

void RPS::callback(cmd *cmdPtr) {
  Robot::getInstance().resetInitialPosition();
  Serial.println("RPS");
}
