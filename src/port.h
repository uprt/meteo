#ifndef PORT_H
#define PORT_H

#include <termios.h>
#include <memory>

#include "config.h"
#include "log.h"
#include "port.h"

namespace meteod {
class Port {
 private:
  const Config config_;
  const Logger logger_;

  int fd_ = -1;  // file descriptor of serial port

  struct termios old_tty_;  // structure to keep serial port original state to
                            // restore it back later

  /**
   * @brief Convert int to speed_t
   * @param speed port baudrate
   * @return port baudrate in speed_t
   */
  speed_t getSpeed(int speed) const;

  /**
   * @brief Convert byte array to hex-string
   * @param data pointer to byte arrat
   * @param len size of byte array to print
   * @return hex-string
   */
  std::string arr2hex(char* data, int len) const;

 public:
  /**
   * @brief Constructor
   * @param app - pointer to parent application class
   */
  Port(const Config& config, const Logger& logger);

  ~Port();

  /**
   * @brief Open and initialize serial port
   * @return true on success, false on error
   */
  bool initialize();

  /**
   * @brief Close and uninitialize serial port
   * @return true on success, false on error
   */
  bool uninitialize();

  /**
   * @brief Flush serial port receive/transmit buffers
   */
  void clearPort() const;

  /**
   * @brief Perform reading from serial port
   * @param fd file descriptor
   * @param buf pointer to pre-allocated data buffer to put received data in
   * @param buflen length og pre-allocated data buffer
   * @param read timeout in milliseconds
   * @return first: true if reading was successful; second: number of read bytes
   * of recieved data
   */
  bool tryRead(void* buf, size_t buflen, int timeout) const;
};
}

#endif
