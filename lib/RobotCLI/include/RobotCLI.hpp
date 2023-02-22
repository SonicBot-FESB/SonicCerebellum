#include <SimpleCLI.h>

class RobotCLI : public SimpleCLI {
public:
  RobotCLI();
  static void sendDistanceSensorData();
  static void sendError(const char *msg);
  static void sendInfo(const char *msg);
  static void sendInfo(const int value);
  static void sendPositionData(const float position, const float rotation);

private:
  static void errorCallback(cmd_error *errorPtr);
};
