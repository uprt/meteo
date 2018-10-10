#include <error.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iomanip>

#include "port.h"

meteod::Port::Port(const Config& config, const Logger& logger)
    : config_(config), logger_(logger) {}

meteod::Port::~Port() { uninitialize(); }

void meteod::Port::clearPort() const { tcflush(fd_, TCIFLUSH); }

speed_t meteod::Port::getSpeed(int speed) const {
  speed_t result = B9600;
  switch (speed) {
    case 4800:
      result = B4800;
      break;
    case 9600:
      result = B9600;
      break;
    case 19200:
      result = B19200;
      break;
    case 38400:
      result = B38400;
      break;
    case 57600:
      result = B57600;
      break;
    case 115200:
      result = B115200;
      break;
    default:
      logger_.log(false, "Incorrect port speed: %d, will use default value",
                  speed);
      break;
  }

  return result;
}

bool meteod::Port::initialize() {
  bool ret = true;

  fd_ = open(config_.port.c_str(), O_RDONLY | O_NOCTTY);
  if (fd_ < 0) {
    logger_.log(false, "Failed to open '%s': Error %d: %s", config_.port, errno,
                strerror(errno));
    ret = false;
  }

  if (ret) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd_, &tty) != 0) {
      logger_.log(false, "Failed to get port parameters: Error %d: %s", errno,
                  strerror(errno));
      ret = false;
      close(fd_);
      fd_ = -1;
    }

    if (ret) {
      old_tty_ = tty;
      speed_t speed = getSpeed(config_.speed);
      cfsetospeed(&tty, speed);
      cfsetispeed(&tty, speed);

      if (config_.parity == 0) {
        tty.c_cflag &= ~PARENB;  // disable parity generation on output and
                                 // parity checking for input
        tty.c_iflag &= ~INPCK;
      } else if (config_.parity == 1) {
        tty.c_cflag &= ~PARENB;  // disable parity generation on output and
                                 // parity checking for input
        tty.c_cflag |= PARODD;
        tty.c_iflag |= INPCK;
      } else if (config_.parity == 2) {
        tty.c_cflag &= ~PARENB;  // disable parity generation on output and
                                 // parity checking for input
        tty.c_cflag &= ~PARODD;
        tty.c_iflag |= INPCK;
      }

      if (config_.double_stopbits == false)
        tty.c_cflag &= ~CSTOPB;  // disable 2nd stopbit
      else
        tty.c_cflag |= CSTOPB;  // enable 2nd stopbit
      tty.c_cflag &= ~CSIZE;
      tty.c_cflag |= CS8;       // will have 8 bits in one byte
      tty.c_cflag &= ~CRTSCTS;  // no flow control
      tty.c_lflag = 0;  // no signaling chars, no echo, no canonical processing
      tty.c_oflag = 0;  // no remapping, no delays
      tty.c_cc[VMIN] = 0;   // minimum size to read (it was getPAcketLenght(),
                            // but for some reason didn't work)
      tty.c_cc[VTIME] = 5;  // 0.5 seconds read timeout between symbols

      tty.c_cflag |=
          CREAD | CLOCAL;  // turn on serial receiver & ignore modem ctrl lines
      tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // turn off any s/w flow ctrl
      tty.c_iflag &= ~(PARMRK | ISTRIP);
      tty.c_iflag |= IGNPAR;
      tty.c_iflag &= ~ICRNL;  // this thing replaced 13 to 10, i missed A LOT OF
                              // time to debug it

      tty.c_lflag &=
          ~(ICANON | ECHO | ECHOE |
            ISIG);  // disable canonical mode (see `man termios 3` for details)
      tty.c_oflag &= ~OPOST;  // turn off implementation-defined i/o mode

      clearPort();

      if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        logger_.log(false, "Failed to set port parameters: Error %d: %s", errno,
                    strerror(errno));
        ret = false;
        close(fd_);
        fd_ = -1;
      }
    }
  }

  if (ret) {
    logger_.log(false, "Port opened successfully");
  }

  return ret;
}

std::string meteod::Port::arr2hex(char* data, int len) const {
  std::ostringstream ss;
  ss << std::hex << std::uppercase << std::setfill('0');
  for (int i = 0; i < len; i++) {
    ss << std::setw(2) << (unsigned int)((unsigned char)data[i]);
  }
  return ss.str();
}

bool meteod::Port::tryRead(void* buf, size_t buflen, int timeout) const {
  fd_set set;
  struct timeval tm;
  FD_ZERO(&set);
  FD_SET(fd_, &set);
  bool result = true;
  size_t readBytes;

  tm.tv_sec = timeout;
  tm.tv_usec = 0;

  int rv = select(fd_ + 1, &set, NULL, NULL, &tm);
  if (rv == -1) {
    logger_.log(false, "Failed to get data from port: Error %d: %s", errno,
                strerror(errno));
    result = false;
  } else if (rv == 0) {
    readBytes = 0;
  } else {
    // small hack, because termios VTIME and VMIN params don't work for some
    // reason
    // and  sometimes Meteo sends data with bad timing (e.g. 8 bytes in one
    // shot, and then other 2 bytes after some small pause)
    usleep(100 * 1000);
    readBytes = read(fd_, buf, buflen);
    logger_.log(true, "Received data from port: %s",
                arr2hex((char*)buf, buflen));
  }

  if (readBytes < buflen) {
    logger_.log(false,
                "Failed to receive data from Meteo: received %d bytes (instead "
                "of %d): %s",
                readBytes, buflen, arr2hex((char*)buf, readBytes));
    result = false;
  }

  return result;
}

bool meteod::Port::uninitialize() {
  if (fd_ >= 0) {
    if (tcsetattr(fd_, TCSANOW, &old_tty_) != 0) {
      logger_.log(false, "Failed restore port parameters: Error %d: %s", errno,
                  strerror(errno));
    }
    close(fd_);
    fd_ = -1;
    logger_.log(false, "Port closed");
  }
  return true;
}
