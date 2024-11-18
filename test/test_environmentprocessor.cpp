#include <gtest/gtest.h>
#include <fatcnt/eventprocessors/EventProcessor.hpp>

class TestEnvironmentProcessor : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Teardown code
    }
};


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}