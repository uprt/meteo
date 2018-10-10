#include <unistd.h>
#include <cassert>
#include <fstream>
#include <memory>

#include "app.h"
#include "listener.h"
#include "port.h"
#include "protocols/proto1.h"

volatile bool meteod::Listener::stop_ = false;

meteod::Listener::Listener(const Config& config, const Logger& logger)
    : config_(config), logger_(logger) {
  switch (config_.proto) {
    default:
      logger_.log(false, "Using default protocol (Type1)");
      // no break, let's fallthrough
    case meteod::Protocol::ProtoType::PROTO_TYPE_TYPE1:
      proto_ = std::make_unique<Type1Protocol>();
      break;
  }
}

meteod::Listener::~Listener() {}

void meteod::Listener::run() {
  const int timeout = config_.silence_timeout;
  const int kPortReopenPause = 1;
  const size_t kMaxBuffer = 256;
  assert(proto_->getPacketLength() < kMaxBuffer);
  meteod::Port port(config_, logger_);
  bool isOk = port.initialize();
  if (!isOk) sleep(kPortReopenPause);
  while (!isStopped() && isOk) {
    uint8_t packet[kMaxBuffer] = {0};
    auto r = port.tryRead(&packet, proto_->getPacketLength(), timeout);
    if (r) {
      meteod::Protocol::ConvertResult result;
      result = proto_->convertPacket(packet);
      if (result.first == Protocol::SUCCESS) {
        logger_.log(true, "Received and parsed data package");
        saveToFile(result.second);
      } else {
        logger_.log(false, "Error: %s", proto_->getErrorMessage(result.first));
        isOk = false;
        port.clearPort();
      }
    }
  }
}

void meteod::Listener::stop() { stop_ = true; }

void meteod::Listener::saveToFile(meteod::Protocol::NormalData data) const {
  if (config_.result_file == "") return;
  std::ofstream resFile(config_.result_file, std::ios_base::out);
  if (resFile.is_open()) {
    resFile << time(NULL) << ";" << data.temp << ";" << data.humidity
            << std::endl;
  } else {
    logger_.log(false, "Failed to open file: %s", config_.result_file);
  }
  resFile.close();
}