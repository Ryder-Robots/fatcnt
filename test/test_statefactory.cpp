#include <gtest/gtest.h>

#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <nlohmann/json.hpp>

using namespace rrobot;
using json = nlohmann::json;

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
    json manifest = {"queues" , {{"limit" , 100}, {"thread_wait_time", 500}, {"thread_process_time", 500}}};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State state = StateFactory::createState(environment);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
