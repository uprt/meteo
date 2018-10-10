#ifndef APP_H
#define APP_H

#include "app.h"
#include "config.h"
#include "log.h"
#include "protocol.h"

namespace meteod {
class App {
 private:
  /**
 * User interrupt requesthandler
 * @param number of raised or received signal
 * Perform app stop (uninitialization) and exit
 */
  static void closeAll(int sig);

  const Config& config_;
  const Logger& logger_;

 public:
  App(const Config& config, const Logger& logger)
      : config_(config), logger_(logger){};

  /**
   * @brief Print some help information
   */
  static void printUsage();

  /**
   * @brief Set signal handlers and print configuration
   */
  void initialize();

  /**
   * @brief Run polling cycle.
   * Run polling cycle: establish connection and poll device for new time
   */
  void runListener() const;
};
}
#endif
