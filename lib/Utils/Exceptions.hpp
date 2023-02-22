class Exception {
public:
  virtual const char *msg() { return "Unimplemented err"; }
};

class GyroFailedException : public Exception {
public:
  const char *msg() { return "Failed to initialize gyro sensor"; }
};