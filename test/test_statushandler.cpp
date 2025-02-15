#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/statushandler.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/state/statemanager.hpp>
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

class TestStatusHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        _manifest = json::parse(ifs);
        ifs.close();
        vector<RRP_QUEUES> directions = {RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE};
        Environment environment = EnviromentProcessor::createEnvironment(_manifest);
        _env = EnviromentProcessor::createEnvironmentRef(_manifest);
        _state = StateFactory::createState(environment, directions);
    }

    void TearDown() override {
        // Teardown code
    }

    vector<RRP_QUEUES> _queueNames = {RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE};
    json _manifest;
    State* _state;
    Environment* _env;
};

class MockHandler : public EventHandler {
   public:
    string name() override { return "MockHandler"; }

    MOCK_METHOD(Event*, produce, (StateIface*), (override));
    MOCK_METHOD(bool, consume, (Event*, StateIface*), (override));
    MOCK_METHOD(bool, available, (), (override));
};

TEST_F(TestStatusHandler, testMSP_IDENT) {
    StateManager* sm = new StateManager(_state, _env);
    RrStatusHandler sh = RrStatusHandler();
    sh.init(_state, _env, sm);
    Event* event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_IN);
    sh.consume(event, _state);
    Event* response = sh.produce(_state);

    msp_ident mspIdent = response->getPayload<msp_ident>();
    EXPECT_EQ(1, mspIdent.get_version());
    EXPECT_EQ(LANDDRONE_4W, mspIdent.get_multitype());
}

TEST_F(TestStatusHandler, testMSP_STATUS) {
    StateManager* sm = new StateManager(_state, _env);
    RrStatusHandler sh = RrStatusHandler();
    sh.init(_state, _env, sm);
    _state->setFlags(RELOADING);
    Event* event = new Event(MSPCOMMANDS::MSP_STATUS, MSPDIRECTION::EXTERNAL_IN);
    sh.consume(event, _state);
    Event* response = sh.produce(_state);

    msp_status mspStatus = response->getPayload<msp_status>();

    EXPECT_EQ(1,1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}