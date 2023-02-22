#include <BaseCommand.hpp>
#include <SimpleCLI.h>

#define TURN_RIGHT 'R'
#define TURN_LEFT 'L'

class FWD : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class BCK : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class TRN : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class POS : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class ENC : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class STP : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class RBT : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};

class RPS : BaseCommand {
public:
  static const void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};
