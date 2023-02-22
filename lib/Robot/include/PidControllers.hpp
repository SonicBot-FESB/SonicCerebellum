#include <PID_v1.h>

#ifndef STRAIGHT_MOVEMENT_PID_KP
#define STRAIGHT_MOVEMENT_PID_KP 20
#endif

#ifndef STRAIGHT_MOVEMENT_PID_KI
#define STRAIGHT_MOVEMENT_PID_KI 10
#endif

#ifndef STRAIGHT_MOVEMENT_PID_KD
#define STRAIGHT_MOVEMENT_PID_KD 1
#endif

#ifndef ROTATING_PID_KP
#define ROTATING_PID_KP 3
#endif

#ifndef ROTATING_PID_KI
#define ROTATING_PID_KI 7
#endif

#ifndef ROTATING_PID_KD
#define ROTATING_PID_KD 1
#endif

#ifndef MOVING_FORWARD
#define MOVING_FORWARD 1
#endif

#ifndef MOVING_BACKWARDS
#define MOVING_BACKWARDS 2
#endif

#define MEASUREMENTS_SAMPLE 30

class MovementPidController {
protected:
  double targetDisplacement;
  double currentDisplacement;
  double strafeRightOutput;
  double strafeLeftOutput;

  PID *strafeLeft;
  PID *strafeRight;

  MovementPidController(int Kp, int Ki, int Kd);

public:
  virtual void setTargetDisplacement(float targetDisplacement) = 0;
  virtual void calibrate(char) = 0;
};

class StraightMovementPidController : public MovementPidController {
private:
  static StraightMovementPidController *instance;
  StraightMovementPidController();
  ~StraightMovementPidController(){};

public:
  static StraightMovementPidController &getInstance();
  void setTargetDisplacement(float targetDisplacement);
  void calibrate(char directionFlag);
};

class TurningPidController : public MovementPidController {
private:
  float measurements[MEASUREMENTS_SAMPLE] = {0};
  int lastMeasurementIndex = 0;
  void pushMeasurement(float value);
  void lShiftMeasurements();
  static TurningPidController *instance;
  TurningPidController();

public:
  static TurningPidController &getInstance();
  void setTargetDisplacement(float targetDisplacement);
  void calibrate(char directionFlag);
  bool isCalibrated();
  ~TurningPidController(){};
};