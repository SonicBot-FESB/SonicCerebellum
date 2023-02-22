#include <Arduino.h>
#include <RobotCLI.hpp>
#include <Robot.hpp>
#include <Exceptions.hpp>

#ifndef BAUD_RATE
#define BAUD_RATE 19200
#endif

RobotCLI *cli;
Robot &r = Robot::getInstance();

void setup()
{
  Serial.begin(BAUD_RATE);
  cli = new RobotCLI();

  r.init();
  r.stop();

  while (1) {
    try {
      r.initGyro();
      break;
    }
    catch (GyroFailedException ex) {
      cli->sendError(ex.msg());
    }
  }
}

uint8_t leftSideMotorSpeed, rightSideMotorSpeed;

void loop()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    cli->parse(input);
  }
  RobotCLI::sendDistanceSensorData();
  delayMicroseconds(1);
}
