#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using namespace rrobot;
using json = nlohmann::json;
namespace fs = std::filesystem;

class TestStateFactory : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

TEST(TestStateFactory, TestStateFactoryCreate) {
    const fs::path filepath = "manifests/virtual.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    // directions are supplied by the handler, and must include queues that are needed by the handler.
    vector<RRP_QUEUES> directions = {RRP_QUEUES::USER_INTERFACE};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);

    EXPECT_EQ(true, state->isRunning());

    queue<Event*>* queueUserInterface = state->getQueues()->getQueue(RRP_QUEUES::USER_INTERFACE);
    delete(state);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
