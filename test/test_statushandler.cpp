#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/statushandler.hpp>
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

class TestStatusHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        _manifest = json::parse(ifs);
        ifs.close();
    }

    void TearDown() override {
        // Teardown code
    }

    vector<RRP_QUEUES> _queueNames = {RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE};
    json _manifest;
};

class MockHandler : public EventHandler {
   public:
    string name() override { return "MockHandler"; }

    MOCK_METHOD(Event*, produce, (StateIface*), (override));
    MOCK_METHOD(bool, consume, (Event*, StateIface*), (override));
    MOCK_METHOD(bool, available, (), (override));

};


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}