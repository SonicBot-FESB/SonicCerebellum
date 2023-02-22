#include <Commands.hpp>
#include <Robot.hpp>
#include <RobotCLI.hpp>

RobotCLI::RobotCLI() {
  FWD::init(this->addCommand("FWD", FWD::callback));
  BCK::init(this->addCommand("BCK", BCK::callback));
  TRN::init(this->addCommand("TRN", TRN::callback));
  POS::init(this->addCommand("POS", POS::callback));
  ENC::init(this->addCommand("ENC", ENC::callback));
  STP::init(this->addCommand("STP", STP::callback));
  RBT::init(this->addCommand("RBT", RBT::callback));
  RPS::init(this->addCommand("RPS", RPS::callback));

  this->setOnError(RobotCLI::errorCallback);
}

void RobotCLI::sendDistanceSensorData() {
  Robot &r = Robot::getInstance();

  uint16_t distanceValues[6];
  r.getDistance(distanceValues[0], distanceValues[1], distanceValues[2],
                distanceValues[3], distanceValues[4], distanceValues[5]);

  Serial.print("DST ");
  Serial.print(distanceValues[0]);
  Serial.print(" ");
  Serial.print(distanceValues[1]);
  Serial.print(" ");
  Serial.print(distanceValues[2]);
  Serial.print(" ");
  Serial.print(distanceValues[3]);
  Serial.print(" ");
  Serial.print(distanceValues[4]);
  Serial.print(" ");
  Serial.println(distanceValues[5]);
}

void RobotCLI::sendInfo(const char *message) {
  Serial.print("INFO ");
  Serial.println(message);
}

void RobotCLI::sendInfo(const int value) {
  Serial.print("INFO ");
  Serial.println(value);
}

void RobotCLI::sendPositionData(const float position, const float rotation) {
  Serial.print("POS ");
  Serial.print(position);
  Serial.print(" ");
  Serial.println(rotation);
}

void RobotCLI::sendError(const char *msg) {
  String msgString = String(msg);
  Serial.println("ERR " + msgString);
}

void RobotCLI::errorCallback(cmd_error *errorPtr) {
  CommandError e(errorPtr);

  Serial.println("ERR " + e.toString());
}
