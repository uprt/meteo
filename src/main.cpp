#include "app.h"
#include "config.h"
#include "log.h"

using namespace meteod;

int main(int argc, char *argv[]) {
  // TODO: make a feature of reading this params from config file, not only from
  // command line
  Config config;
  config.parseCMDLine(argc, argv);
  Logger logger(config.debug_log);

  App app(config, logger);
  app.initialize();
  app.runListener();
  return 0;
}
