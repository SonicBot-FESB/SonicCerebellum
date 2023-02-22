/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for
   details. Source: github.com/spacehuhn/SimpleCLI
 */

// Watch the tutorial here: https://youtu.be/UyW-wICdnKo

#include <Arduino.h>
#include <DistanceSensor.hpp>
#include <MotorWithEncoder.hpp>
#include <Robot.hpp>
#include <RobotCLI.hpp>

RobotCLI cli;

Motor m1 = Motor(5, 3);
Motor m2 = Motor(8, 6);

MotorWithEncoder m4 = MotorWithEncoder(4, 2, 25, 27);
MotorWithEncoder m3 = MotorWithEncoder(9, 7, 29, 31);

int IR_PINS[] = {
    PIN_A14, // 1
    PIN_A15, // 2
    PIN_A16, // 4
    PIN_A3,  // 3
    PIN_A5,  // 5
    PIN_A4,  // 6
};

DistanceSensor sensors[] = {
    // DistanceSensor(IR_PINS[0]),
    // DistanceSensor(IR_PINS[1]),
    // DistanceSensor(IR_PINS[2]),
    DistanceSensor(IR_PINS[3]),
    // DistanceSensor(IR_PINS[4]),
    DistanceSensor(IR_PINS[5]),
};

void m3_encoder_ISR() { m3.encoderCount(); }

void m4_encoder_ISR() { m4.encoderCount(); }

void setup() {
  Serial.begin(19200);
  attachInterrupt(m4.interruptPin, m4_encoder_ISR, FALLING);
  attachInterrupt(m3.interruptPin, m3_encoder_ISR, FALLING);
}

void loop() {
  // if (Serial.available())
  // {
  //   String input = Serial.readStringUntil('\n');
  //   cli.parse(input);
  // }

  for (DistanceSensor &s : sensors) {
    Serial.print(s.read());
    Serial.print(" : ");
  }
  Serial.println();
  delay(100);
}