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
    payload->set_pitch(0);
    payload->set_yaw(0.5);
    payload->set_throttle(1);
    payload->set_aux1(0);
    payload->set_aux2(0);
    payload->set_aux3(0);
    payload->set_aux4(0);
    Event* event = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, payload);

    matrix<uint8_t> m = _msp10Ctl.deserialize(event);

    uint8_t v = m(0,0);
    // 127 is 255/2 therfore equal to 0.
    EXPECT_EQ(127, m(0, 0));
    EXPECT_EQ(127, m(0, 1));
    EXPECT_EQ(191, m(0, 2));
    EXPECT_EQ(255, m(0, 3));
    EXPECT_EQ(127, m(0, 4));
    EXPECT_EQ(127, m(0, 5));
    EXPECT_EQ(127, m(0, 6));
    EXPECT_EQ(127, m(0, 7));
}