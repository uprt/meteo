#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace meteod {

struct Config {
  std::string result_file;  // file path to save data from last successfull poll
  std::string port = "/dev/ttyS0";  // port name, e.g. /dev/ttyS1
  int speed = 9600L;                // port speed
  bool double_stopbits = true;      // if true, will use 2 stopbits
  int parity = 0;                   // none, odd, even
  int proto = 0;                    // now can be only '0'
  int silence_timeout = 60 * 60;
  bool debug_log = false;

  /**
   * @brief Parse command line and initialize config variables
   * @param argc number of args
   * @param argv pointer to the array of args
   * @return parsed configuration
   */
  void parseCMDLine(int argc, char* argv[]);
};
}

#endif
