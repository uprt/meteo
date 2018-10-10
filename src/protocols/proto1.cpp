#include "proto1.h"

bool meteod::Type1Protocol::checkChecksum(type1packet* const packet) const {
  constexpr size_t pksize = sizeof(type1packet);
  static_assert(pksize > 2, "Bad packet structure");

  int i_xor = packet->blob[0];
  for (size_t i = 1; i < pksize; i++) {
    i_xor = i_xor ^ packet->blob[i];
  }

  return (i_xor == 0);
}

meteod::Protocol::ConvertResult meteod::Type1Protocol::convertPacket(
    uint8_t* const data) const {
  const int8_t kMinTemp = -50;
  const int8_t kMaxTemp = 50;

  // here and above could be aliasing rules or 'active member' rule violation
  // but it works and I don't care for now
  // TODO: rewrite it
  type1packet* packet = reinterpret_cast<type1packet*>(data);

  int result;
  NormalData convertedData{0, 0};

  result = SUCCESS;
  if (packet->fields.status != 0xF7) {
    result = STATUS_NOT_OK;
  } else if (checkChecksum(packet) == false) {
    result = BAD_CHECKSUM;
  }

  if (result == SUCCESS) {
    convertedData.temp =
        strangeHex2Int(packet->fields.temp) * (packet->fields.sign ? 1 : -1);
    convertedData.humidity = strangeHex2Int(packet->fields.humidity);

    if (convertedData.temp > kMaxTemp || convertedData.temp < kMinTemp) {
      result = WRONG_DATA;
    }
  }

  return {result, convertedData};
}

std::string meteod::Type1Protocol::getErrorMessage(int errNo) const {
  switch (errNo) {
    case STATUS_NOT_OK:
      return "Wrong packet: status byte is not  0xF7";
      break;
    case BAD_CHECKSUM:
      return "Wrong packet: incorrect checksum!";
      break;
    case WRONG_DATA:
      return "Wrong packet: incorrect temperature value";
    default:
      return "Unknown error";
  }
}

size_t meteod::Type1Protocol::getPacketLength() const {
  return sizeof(type1packet);
}
