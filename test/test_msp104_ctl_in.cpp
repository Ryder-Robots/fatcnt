#include <gtest/gtest.h>
#include <fatcnt/events/msp104/msp104_ctl_in.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>

using namespace rrobot;

class TestMsp104In : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }

    Msp104Ctl _msp10Ctl;
};

TEST_F(TestMsp104In, TestDeserialize) {
    msp_motor* payload = new msp_motor();

    payload->set_roll(0);
    payload->set_pitch(1);
    payload->set_yaw(0);
    payload->set_throttle(500);
    payload->set_aux1(0);
    payload->set_aux2(0);
    payload->set_aux3(0);
    payload->set_aux4(0);
    Event* event = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, payload);

    std::vector<uint8_t> m = _msp10Ctl.deserialize(event);

    // 0 = roll
    // 1 = pitch
    // 2 = yaw
    // 3 = throttle

    EXPECT_EQ(127, m[0]);
    EXPECT_EQ(255, m[1]);
    EXPECT_EQ(127, m[2]);
    EXPECT_EQ(134, m[3]);
    EXPECT_EQ(127, m[4]);
    EXPECT_EQ(127, m[5]);
    EXPECT_EQ(127, m[6]);
    EXPECT_EQ(127, m[7]);
}