#include <gtest/gtest.h>

#include <fatcnt/state/statefactory.hpp>

using namespace rrobot;

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

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
