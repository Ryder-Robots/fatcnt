#include <gtest/gtest.h>
#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>

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
    // 0 = roll
    // 1 = pitch
    // 2 = yaw
    // 3 = throttle

    msp_motor* mspMotor = new msp_motor();
    mspMotor->set_roll(0);
    mspMotor->set_pitch(1);
    mspMotor->set_yaw(0);
    mspMotor->set_throttle(500);
    Event* msp104 = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, mspMotor);
    Event* event = _hbridgeSerializer.convert(msp104);

    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b1010, payload.get_in());
    EXPECT_EQ(500, payload.get_motor1());
    EXPECT_EQ(500, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());
}

TEST_F(TestHbridgeSerializer, TestBackward) {
    msp_motor* mspMotor = new msp_motor();
    mspMotor->set_roll(0);
    mspMotor->set_pitch(-1);
    mspMotor->set_yaw(0);
    mspMotor->set_throttle(500);
    Event* msp104 = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, mspMotor);
    Event* event = _hbridgeSerializer.convert(msp104);
    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b0101, payload.get_in());
    EXPECT_EQ(500, payload.get_motor1());
    EXPECT_EQ(500, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());   
}

TEST_F(TestHbridgeSerializer, TestLeftForward) {
    //std::vector<uint8_t> m = {0,255,0,127,0,0,0,0};
    msp_motor* mspMotor = new msp_motor();
    mspMotor->set_roll(0);
    mspMotor->set_pitch(1);
    mspMotor->set_yaw(-1);
    mspMotor->set_throttle(500);
    Event* m = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, mspMotor);
    Event* event = _hbridgeSerializer.convert(m);

    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b0110, payload.get_in());
    EXPECT_EQ(500, payload.get_motor1());
    EXPECT_EQ(500, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());   
}

TEST_F(TestHbridgeSerializer, TestRightForward) {
    msp_motor* mspMotor = new msp_motor();
    mspMotor->set_roll(0);
    mspMotor->set_pitch(1);
    mspMotor->set_yaw(1);
    mspMotor->set_throttle(500);
    Event* m = new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_IN, mspMotor);
    Event* event = _hbridgeSerializer.convert(m);

    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();

    EXPECT_EQ(0b1001, payload.get_in());
    EXPECT_EQ(500, payload.get_motor1());
    EXPECT_EQ(500, payload.get_motor2());
    EXPECT_EQ(0, payload.get_motor3());
    EXPECT_EQ(0, payload.get_motor4());   
}

