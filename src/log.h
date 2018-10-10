#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <sstream>
#include <string>

namespace meteod {
class Logger {
 private:
  bool debug_log_ = false;

 public:
  Logger(bool debug_log) : debug_log_(debug_log){};

  void log(const char* s) const {
    while (*s) {
      std::cout << *s++;
    }
    std::cout << std::endl;
  }

  void log(bool is_debug, const char* s) const {
    auto keep_silence = is_debug && !debug_log_;
    if (keep_silence) return;
    log(s);
  }

  template <typename T, typename... Args>
  void log(bool is_debug, const char* s, const T& value,
           const Args&... args) const {
    auto keep_silence = is_debug && debug_log_;
    if (keep_silence) return;
    log(s, value, args...);
  }

  template <typename T, typename... Args>
  void log(const char* s, const T& value, const Args&... args) const {
    while (*s) {
      if (*s == '%' && *++s != '%') {
        std::cout << value;
        return log(++s, args...);
      }
      std::cout << *s++;
    }
  }
};
}

#endif