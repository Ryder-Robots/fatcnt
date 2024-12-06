#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/ui/jseralizer.hpp>
#include <fatcnt/events/uihandler.hpp>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

using ::testing::Return;
using ::testing::_;
using ::testing::AnyNumber;

class TestUiHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

class MockExternal : public External {
    public:
    MockExternal() {
        json inbound;
        inbound["command"] = "MSP_SET_MOTOR_HBRIDGE";
        json payload;
        payload["motor1"] = 500;
        payload["motor2"] = 600;

        payload["motor3"] = 500;
        payload["motor4"] = 600;

        // set polarity of motors
        payload["in"] = 0b01010101;

        inbound["payload"] = payload;
        _response = inbound.dump() + string(1, 0x1E) + inbound.dump();
    }

    ssize_t recv_rr(void* buffer, size_t bufsz) override {
        memcpy(buffer, &(_response.c_str()[_pointer++]), sizeof(char));
        return 1;
    }

    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void *buf, size_t bufsz), (override));
    MOCK_METHOD(size_t, available, (), (override));

    string _response;

    private:
    int _pointer = 0;
   
};

TEST(TestUniHandler, TestInit) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<MSPDIRECTION> directions = {MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();

    External* external = new MockExternal();

    uihandler.init(external, state, serializer);    
}

// Test 4 motor setup using dual h-bridge
TEST(TestUiHandler, TestInBoundEvents) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<MSPDIRECTION> directions = {MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();

    MockExternal external = MockExternal();
    uihandler.init(&external, state, serializer);

    EXPECT_CALL(external, available())
        .Times(AnyNumber())
        .WillOnce(Return(external._response.size()))
        .WillOnce(Return(1))
        .WillOnce(Return(0))
        .WillOnce(Return(0));
        
    thread t(&UiHandler::handleEvent, &uihandler, state);
    // if (t.joinable()) {
    //     t.detach();
    // }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    state->setIsRunning(false);

    while (!t.joinable()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (t.joinable()) {
        t.join();
    }

    // Get the queue that events should be written to
    queue<Event*>* thisQueue = state->getQueues()->getQueue(MSPDIRECTION::CATEGORIZER);
    size_t sz = thisQueue->size();

    EXPECT_EQ(2, sz);
    Event* event1 = thisQueue->front();
    thisQueue->pop();
    Event* event2 = thisQueue->front();
    thisQueue->pop();

    msp_set_motor_hbridge payload1 = event1->getPayload<msp_set_motor_hbridge>(), 
        payload2 = event2->getPayload<msp_set_motor_hbridge>();


    EXPECT_EQ(500, payload1.get_motor1());
    EXPECT_EQ(600, payload1.get_motor2());
    EXPECT_EQ(500, payload1.get_motor3());
    EXPECT_EQ(600, payload1.get_motor4());
    EXPECT_EQ(0b01010101, payload1.get_in());

    EXPECT_EQ(500, payload2.get_motor1());
    EXPECT_EQ(600, payload2.get_motor2());
    EXPECT_EQ(500, payload2.get_motor3());
    EXPECT_EQ(600, payload2.get_motor4());
    EXPECT_EQ(0b01010101, payload2.get_in());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
