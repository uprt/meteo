#ifndef PROTO1_H
#define PROTO1_H

#include "protocol.h"

namespace meteod {
class Type1Protocol : public Protocol {
 public:
  Type1Protocol() = default;

  enum Protocol1ParseError {
    STATUS_NOT_OK = -2,
    BAD_CHECKSUM = -3,
    WRONG_DATA = -4
  };

  union type1packet {
    struct __attribute__((__packed__)) {
      uint8_t magic;
      uint8_t status;
      uint8_t sign;
      uint8_t temp;
      uint8_t humidity;
      uint8_t checksum;
    } fields;
    uint8_t blob[sizeof(fields)];
  };

  /**
   * @brief return the length of the packet we need to read from port
   * @return length of the packet
   */
  virtual size_t getPacketLength() const override;

  /**
   * @brief return error message string depending on error code
   * @return error mesage string
   */
  std::string getErrorMessage(int errNo) const override;

  /**
   * @brief Check received packet for format compliant and adequate data and
   * converts it to normalData
   * @return MeteoProtocol::SUCCESS if okay, or error code if not
   */
  virtual ConvertResult convertPacket(uint8_t* const data) const override;

 private:
  bool checkChecksum(type1packet* const packet) const;

  /**
   * @brief Convert 8-bit ints from `strange hex` (realy something strange) for
   * meteo protocols
   * @param int to convert
   * @return result of the conversion
   */
  inline uint8_t strangeHex2Int(uint8_t byte) const {
    return (byte / 16) * 10 + byte % 16;
  }
};
}

#endif
