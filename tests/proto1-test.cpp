#include "proto1.h"
#include <gtest/gtest.h>

class TestProto1Parser : public ::testing::Test {
 public:
  TestProto1Parser() {}
  ~TestProto1Parser() {}
  void SetUp() {}
  void TestProto1() {}
  meteod::Type1Protocol proto_;
};

TEST_F(TestProto1Parser, StatusInvalid) {
  uint8_t dump[] = {0x00, 0xFF, 0x01, 0x05, 0x75, 0x8E};
  auto r = proto_.convertPacket(&dump[0]);
  ASSERT_EQ(meteod::Type1Protocol::STATUS_NOT_OK, r.first);
}

TEST_F(TestProto1Parser, BadCRC) {
  uint8_t dump[] = {0x00, 0xF7, 0x01, 0x05, 0x75, 0xFF};
  auto r = proto_.convertPacket(&dump[0]);
  ASSERT_EQ(meteod::Type1Protocol::BAD_CHECKSUM, r.first);
}

TEST_F(TestProto1Parser, TempPlusOK) {
  uint8_t dump[] = {0x00, 0xF7, 0x01, 0x31, 0x75, 0xB2};
  auto r = proto_.convertPacket(&dump[0]);
  ASSERT_EQ(meteod::Type1Protocol::SUCCESS, r.first);
  EXPECT_EQ(r.second.temp, 31);
  EXPECT_EQ(r.second.humidity, 75);
}

TEST_F(TestProto1Parser, TempMinusOK) {
  uint8_t dump[] = {0x00, 0xF7, 0x00, 0x31, 0x74, 0xB2};
  auto r = proto_.convertPacket(&dump[0]);
  ASSERT_EQ(meteod::Type1Protocol::SUCCESS, r.first);
  EXPECT_EQ(r.second.temp, -31);
  EXPECT_EQ(r.second.humidity, 74);
}

TEST_F(TestProto1Parser, TempBad) {
  uint8_t dump[] = {0x00, 0xF7, 0x01, 0x60, 0x75, 0xE3};
  auto r = proto_.convertPacket(&dump[0]);
  EXPECT_EQ(meteod::Type1Protocol::WRONG_DATA, r.first);

  uint8_t dump2[] = {0x00, 0xF7, 0x00, 0x60, 0x75, 0xE2};
  r = proto_.convertPacket(&dump2[0]);
  EXPECT_EQ(meteod::Type1Protocol::WRONG_DATA, r.first);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}