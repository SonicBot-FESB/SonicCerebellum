#include <SimpleCLI.h>

class BaseCommand {
public:
  const static void init(const Command &cmd);
  static void callback(cmd *cmdPtr);
};