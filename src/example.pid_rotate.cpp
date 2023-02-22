#include <Arduino.h>

#include <math.h>

#include <PID_v1.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

const int M3_B = 31;
const int M3_A = 29;
const int M4_B = 27;
const int M4_A = 25;

const int MOTOR_PINS[] = {
    3, 5, // M1 -- RIGHT
    6, 8, // M2 -- LEFT
    7, 9, // M3 -- RIGHT
    2, 4, // M4 -- LEFT
};

const int GYRO_SCL = 37;
const int GYRO_SDA = 38;

double initial_rotation = 359.94;
double initial_displacement = 360 - 359.94;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1);

double setpoint = 90, input, output_strafe_right, output_strafe_left;
double Kp = 3, Ki = 8, Kd = 0;

PID calibration_strafe_right(&input, &output_strafe_right, &setpoint, Kp, Ki,
                             Kd, REVERSE);
PID calibration_strafe_left(&input, &output_strafe_left, &setpoint, Kp, Ki, Kd,
                            DIRECT);

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

double get_rotation() {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  return event.orientation.x;
}

int t1 = millis();

void setup() {
  Wire1.setSCL(37);
  Wire1.setSDA(38);

  Serial.begin(9600);

  for (int pin : MOTOR_PINS) {
    pinMode(pin, OUTPUT);
  }

  bool gyro_setup = init_gyro();
  if (!gyro_setup) {
    while (1)
      ;
  }

  for (int i = 0; i < 15; i++) {
    initial_rotation = get_rotation();
    delay(100);
  }

  calibration_strafe_left.SetMode(AUTOMATIC);
  calibration_strafe_right.SetMode(AUTOMATIC);
}

int const start_speed = 250;

void loop() {
  double rotation = get_rotation();
  double displacement = -1;

  if (rotation > 180) {
    displacement = rotation - 360 + initial_displacement;
  } else {
    displacement = rotation + initial_displacement;
  }

  displacement = ceill(displacement * 100) / 100;

  input = ceil(displacement);
  calibration_strafe_left.Compute();
  calibration_strafe_right.Compute();

  int speed = (output_strafe_left - output_strafe_right) * 0.5;

  Serial.print(displacement);
  Serial.print(" ");
  Serial.print(output_strafe_left);
  Serial.print(" ");
  Serial.print(output_strafe_right);
  Serial.print("\n");

  if (speed > 0) {
    if (speed < 50)
      speed = 50;
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
  } else if (speed < 0) {
    speed = -speed;
    if (speed < 50)
      speed = 50;
    // LEFT
    analogWrite(6, 0);
    analogWrite(2, 0);
    analogWrite(8, speed);
    analogWrite(4, speed);

    // RIGHT
    analogWrite(5, 0);
    analogWrite(9, 0);
    analogWrite(3, speed);
    analogWrite(7, speed);
  }

  if (displacement > 89 && displacement < 91) {
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
  }
}