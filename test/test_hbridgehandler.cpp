#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/hbridgecontroller.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

using ::testing::Return;

class TestHbridgeHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        _manifest = json::parse(ifs);
        ifs.close();

        vector<RRP_QUEUES> directions = {RRP_QUEUES::MICROCONTROLLER, RRP_QUEUES::CATEGORIZER};
        _environment = EnviromentProcessor::createEnvironmentRef(_manifest);
        _state = StateFactory::createState(*_environment, directions);
    }

    void TearDown() override {
        // Teardown code
    }

    vector<RRP_QUEUES> _queueNames = {RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE};
    json _manifest;
    Environment* _environment = nullptr;
    State* _state = nullptr;
    vector<RRP_QUEUES> _directions;
};

TEST_F(TestHbridgeHandler, TestInit) {
    HbridgeController hbridge = HbridgeController();
    hbridge.init(_state, _environment);
    hbridge.setUp();
}

TEST_F(TestHbridgeHandler, TestCommand) {
    HbridgeController hbridge = HbridgeController();

    hbridge.init(_state, _environment);
    hbridge.setUp();

    msp_set_motor_hbridge* payload = new msp_set_motor_hbridge();
    payload->set_in(0b1010);
    payload->set_motor1(500);
    payload->set_motor2(500);

    Event* event = new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN, payload);
    EXPECT_EQ(true, hbridge.consume(event, _state));

    event = hbridge.produce(_state);
    msp_set_motor_hbridge result = event->getPayload<msp_set_motor_hbridge>();
    EXPECT_EQ(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, event->getCommand());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}