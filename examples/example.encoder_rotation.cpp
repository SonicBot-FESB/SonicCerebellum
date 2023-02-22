#include <Arduino.h>
#include <consts.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

volatile long unsigned int count_fwd = 0;
volatile long unsigned int count_bck = 0;

const int GYRO_SCL = 37;
const int GYRO_SDA = 38;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1);

sensors_event_t event;
double get_rotation() {
  /* Get a new sensor event */
  bno.getEvent(&event);

  return event.orientation.x;
}

void encoder_left_ISR() {
  int M3_A_val = digitalRead(M3_A);
  count_fwd += 1 - M3_A_val;
  count_bck += M3_A_val;
}

bool init_gyro() {
  Serial.println("Orientation Sensor Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print(
        "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    return false;
  }

  delay(1000);

  bno.setExtCrystalUse(true);
  Serial.println("Gyro working");
  return true;
}

double initial_rotation = 359.94;
double initial_displacement = 360 - 359.94;

void setup() {
  Serial.begin(9600);
  for (int pin : ENCODER_PINS) {
    pinMode(pin, INPUT_PULLDOWN);
  }
  for (int pin : MOTOR_PINS) {
    pinMode(pin, OUTPUT);
  }
  attachInterrupt(M3_B, encoder_left_ISR, FALLING);

  bool gyro_setup = init_gyro();
  if (!gyro_setup) {
    while (1)
      ;
  }

  for (int i = 0; i < 15; i++) {
    initial_rotation = get_rotation();
    delay(100);
  }
}

void loop() {
  double rotation = get_rotation();
  double displacement = -1;

  if (rotation > 180) {
    displacement = rotation - 360 + initial_displacement;
  } else {
    displacement = rotation + initial_displacement;
  }

  displacement = ceill(displacement * 100) / 100;

  int speed = 60;

  if (count_bck >= 515) {
    // LEFT
    analogWrite(6, 0);
    analogWrite(2, 0);
    analogWrite(8, 0);
    analogWrite(4, 0);

    // RIGHT
    analogWrite(5, 0);
    analogWrite(9, 0);
    analogWrite(3, 0);
    analogWrite(7, 0);
  } else {
    // LEFT
    analogWrite(6, speed);
    analogWrite(2, speed);
    analogWrite(8, 0);
    analogWrite(4, 0);

    // RIGHT
    analogWrite(5, speed);
    analogWrite(9, speed);
    analogWrite(3, 0);
    analogWrite(7, 0);
  }

  Serial.print(displacement);
  Serial.print(" - ");
  Serial.print(count_bck);
  Serial.println();
}