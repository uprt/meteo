#ifndef LISTENER_H
#define LISTENER_H

#include <memory>

#include "config.h"
#include "log.h"
#include "protocol.h"

namespace meteod {
class Listener {
 private:
  const Config config_;
  const Logger logger_;
  std::unique_ptr<Protocol> proto_;

  /**
   * @brief Write last successfull data to the file
   */
  void saveToFile(meteod::Protocol::NormalData data) const;

  static volatile bool stop_;  // flag to break the main cycle. should be
                               // volatile to avoid optimizations

 public:
  /**
   * @brief Constructor
   * @param app - pointer to parent application class
   */
  Listener(const Config& config, const Logger& logger);

  ~Listener();

  /**
   * @brief Run polling cycle.
   * Run polling cycle. Method will return if too many erorrs happened
   */
  void run();

  /**
   * @brief Emergency stop for all instances :)
   */
  static void stop();

  bool isStopped() const { return stop_; }
};
}

#endif
