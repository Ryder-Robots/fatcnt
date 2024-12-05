#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/ui/jseralizer.hpp>
#include <fatcnt/events/uihandler.hpp>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

using ::testing::Return;
using ::testing::_;

class TestUiHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

// mock authenticator
class MockAuthenticator : public Authenticator {
    MOCK_METHOD(bool, authenticate, (Event*), (override));
};

class MockExternal : public External {
    //  ssize_t recv_rr(void* buffer, size_t bufsz)
    //  int accept_rr(bool accept);
    // void close_rr();
    // ssize_t send_rr(const void *buf, size_t bufsz);
    //  size_t available();

    MOCK_METHOD(ssize_t, recv_rr, (void* buffer, size_t bufsz), (override));
    MOCK_METHOD(int, accept_rr, (bool accept), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void *buf, size_t bufsz), (override));
    MOCK_METHOD(size_t, available, (), (override));
};

TEST(TestUiHandler, TestInBoundEvents) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<MSPDIRECTION> directions = {MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json> serializer = Jseralizer();

    MockAuthenticator* authenticator = new MockAuthenticator();
    External* external = new MockExternal();

    // uihandler.init(authenticator, external, state, serializer);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
