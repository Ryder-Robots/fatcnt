#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <fatcnt/state/statefactory.hpp>
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
    vector<MSPDIRECTION> directions = {MSPDIRECTION::USER_INTERFACE};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);

    EXPECT_EQ(true, state->isRunning());
    EXPECT_EQ(false, state->isAuthenticated());

    queue<Event*>* queueUserInterface = state->getQueues()->getQueue(MSPDIRECTION::USER_INTERFACE);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
