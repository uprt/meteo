#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include <utility>

namespace meteod {
class Protocol {
 public:
  enum ProtoType { PROTO_TYPE_TYPE1 = 0 };

  enum ParselResult { GENERAL_ERROR = -1, SUCCESS = 0 };

  struct NormalData {
    int8_t temp;
    uint16_t humidity;
  };

  using ConvertResult = std::pair<int, NormalData>;

  virtual ~Protocol(){};

  /**
   * @brief return the length of the packet we need to read from port
   * @return length of the packet
   */
  virtual size_t getPacketLength() const = 0;

  /**
   * @brief return error message string depending on error code
   * @return error mesage string
   */
  virtual std::string getErrorMessage(int errNo) const = 0;

  /**
   * @brief Check received packet for format compliant and adequate data and
   * converts it to normalData
   * @return true if packet is good, otherwise false (and nData is not changed)
   */
  virtual ConvertResult convertPacket(uint8_t* const data) const = 0;
};
}

#endif
