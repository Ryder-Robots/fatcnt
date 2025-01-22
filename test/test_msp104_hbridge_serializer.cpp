#include <gtest/gtest.h>
#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

using namespace rrobot;

class TestHbridgeSerializer : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }

    Msp104Serializer _hbridgeSerializer;
};

TEST_F(TestHbridgeSerializer, TestForward) {
    // pitch = 0 : 127
    // yaw = -1 : 0
    // throttle 500 : 0

    std::vector<uint8_t> m = {255,0,128,127,0,0,0,0};
    Event* event = _hbridgeSerializer.deserialize(m);

    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b1010, payload.get_in());
    EXPECT_EQ(498, payload.get_motor1());
    EXPECT_EQ(498, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());
}

TEST_F(TestHbridgeSerializer, TestBackward) {
    std::vector<uint8_t> m = {0,0,128,127,0,0,0,0};
    Event* event = _hbridgeSerializer.deserialize(m);

    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b0101, payload.get_in());
    EXPECT_EQ(498, payload.get_motor1());
    EXPECT_EQ(498, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());   
}
