#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "app.h"
#include "config.h"
#include "listener.h"

void meteod::App::closeAll(int /*sig*/) {
  std::cout << "Interrupt signal catched, shutting down..." << std::endl;
  Listener::stop();
}

void meteod::App::initialize() {
  signal(SIGTERM, App::closeAll);
  signal(SIGINT, App::closeAll);

  logger_.log(false, "MeteoD build on _%s %s started", __DATE__, __TIME__);
  logger_.log(false, "Using port: %s", config_.port);
  logger_.log(false, "Using port speed: %lu", config_.speed);
  logger_.log(false, "Using %d stop-bits", config_.double_stopbits ? 2 : 1);
  logger_.log(false, "Using parity mode %d (0 - none, 1 - odd, 2 - even)",
              config_.parity);
  logger_.log(false, "Using silence timeout %u sec.", config_.silence_timeout);
  if (config_.debug_log)
    logger_.log(false, "Using verbose logging");
}

void meteod::App::printUsage() {
  std::cout << "MeteoD command line options:" << std::endl;
  std::cout << "-v (--verbose): print additional log information to console"
            << std::endl;
  std::cout << "-p (--port): COM-port to communicate" << std::endl;
  std::cout << "-s (--speed): COM-port speed (e.g. 9600)" << std::endl;
  std::cout << "-b (--stopbits): set stopbits count (1 or 2)" << std::endl;
  std::cout << "-r (--parity): setparity mode:  0 - none, 1 - odd, 2 - even"
            << std::endl;
  std::cout << "-f (--file): file path to save data" << std::endl;
  std::cout
      << "-t (--timeout): 'silence timeout' in sec. to reopen port if no data"
      << std::endl;
  std::cout << "-i (--protocol): communication protocol to use (0 = 'Type1')"
            << std::endl;
}

void meteod::App::runListener() const {
  Listener listener(config_, logger_);
  while (!listener.isStopped()) {
    logger_.log(true, "Port listener initialized, starting cycle...");
    listener.run();
  }
  logger_.log(false, "MeteoD finished it's work.");
}
