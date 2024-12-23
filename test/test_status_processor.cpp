#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/state/statusprocessor.hpp>
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

class TestStatusProcessor : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        _manifest = json::parse(ifs);
        ifs.close();
        Environment environment = EnviromentProcessor::createEnvironment(_manifest);
        _state = StateFactory::createState(environment, _queueNames);
    }

    void TearDown() override {
        // Teardown code
    }

    vector<RRP_QUEUES> _queueNames = {RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE};
    json _manifest;
    StateIface* _state;    
};

class MockHandler : public EventHandler {
   public:
    string name() override { return _name; }
    MOCK_METHOD(Event*, produce, (StateIface*), (override));
    MOCK_METHOD(bool, consume, (Event*, StateIface*), (override));
    MOCK_METHOD(bool, available, (), (override));

    string _name = "MockHalder";
};

TEST_F(TestStatusProcessor, TestGetFlags) {
    MockHandler* m1 = new MockHandler();
    MockHandler* m2 = new MockHandler();
    MockHandler* m3 = new MockHandler();
    vector<EventHandler*> handlers = {m1, m2, m3};
    _state->setFlags(0);

    StatusProcessor* statusProcessor = new StatusProcessor(_state);
    statusProcessor->addHandler(m1);
    statusProcessor->addHandler(m2);
    statusProcessor->addHandler(m3);
    statusProcessor->setMode(CMODE_MANUAL_FLIGHT);

    m1->setStatus(RRP_STATUS::ACTIVE);
    m2->setStatus(RRP_STATUS::ACTIVE);
    m3->setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::ACTIVE, statusProcessor->getStatus());

    m1->setStatus(RRP_STATUS::TERMINATED);
    m2->setStatus(RRP_STATUS::ERROR);
    m3->setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::TERMINATED, statusProcessor->getStatus());

    m1->setStatus(RRP_STATUS::ERROR);
    m2->setStatus(RRP_STATUS::RELOADING);
    m3->setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::ERROR, statusProcessor->getStatus());
    EXPECT_EQ(CMODE_MANUAL_FLIGHT, statusProcessor->getMode());

    statusProcessor->setMode(CMODE_NOT_SET);
    EXPECT_EQ(CMODE_NOT_SET, statusProcessor->getMode());
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}