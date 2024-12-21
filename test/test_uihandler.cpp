#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/events/ui/jseralizer.hpp>
#include <fatcnt/events/uihandler.hpp>
#include <fatcnt/protocols/common/mspcommands.hpp>

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

        // second message
        json inbound2;
        inbound2["command"] = "MSP_SET_MOTOR_HBRIDGE";
        json payload2;
        payload2["motor1"] = 500;
        payload2["motor2"] = 600;

        payload2["motor3"] = 500;
        payload2["motor4"] = 600;

        // set polarity of motors
        payload2["in"] = 0b01010101;

        inbound2["payload"] = payload;

        // test an ident request.
        json msp_ident;
        msp_ident["command"] = "MSP_IDENT";
        _response = inbound.dump() + string(1, 0x1E) + inbound2.dump() + string(1, 0x1E) + msp_ident.dump() +  string(1, 0x1E);
    }

    virtual ssize_t recv_rr(void* buffer, size_t bufsz) override {
        memcpy(buffer, &(_response.c_str()[_pointer++]), sizeof(char));
        return 1;
    }

    virtual ssize_t send_rr(const void *buf, size_t bufsz) {
        _outbound = string(reinterpret_cast<const char*>(buf));
        return bufsz;
    }

    virtual ssize_t available() {
        return _response.size() - _pointer;
    }

    void init(Environment* environment, StateIface* _state) override {}

    virtual void close_rr() override {};

    MOCK_METHOD(int, accept_rr, (), (override));

    string _response;
    string _outbound = "";

    MOCK_METHOD(int, shutdown_rr, (), (override));

    private:
    int _pointer = 0;   
};

class MockExternal2 : public External {
    public:
    MOCK_METHOD(ssize_t,recv_rr, (void*, size_t), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void*, size_t), (override));
    MOCK_METHOD(void, close_rr, (), (override));
    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(ssize_t, available, (), (override));
    MOCK_METHOD(void, init, (Environment*, StateIface*), (override));
    MOCK_METHOD(int, shutdown_rr, (), (override));
};

TEST(TestUniHandler, TestInit) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<RRP_QUEUES> directions = {RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    Environment* env = EnviromentProcessor::createEnvironmentRef(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();

    External* external = new MockExternal();

    uihandler.init(external, env, state, serializer);
}

// Test 4 motor setup using dual h-bridge
TEST(TestUiHandler, TestInBoundEvents) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<RRP_QUEUES> directions = {RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();

    MockExternal external = MockExternal();
    uihandler.init(&external, &environment, state, serializer);
        
    thread t(&UiHandler::handleEvent, &uihandler, state);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    state->setIsRunning(false);

    while (!t.joinable()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (t.joinable()) {
        t.join();
    }

    // Get the queue that events should be written to
    queue<Event*>* thisQueue = state->getQueues()->getQueue(RRP_QUEUES::CATEGORIZER);
    size_t sz = thisQueue->size();

    EXPECT_EQ(3, sz);
    Event* event1 = thisQueue->front();
    thisQueue->pop();
    Event* event2 = thisQueue->front();
    thisQueue->pop();
    Event* event3 = thisQueue->front();
    thisQueue->pop();

    msp_set_motor_hbridge payload1 = event1->getPayload<msp_set_motor_hbridge>(), 
        payload2 = event2->getPayload<msp_set_motor_hbridge>();


    EXPECT_EQ(500, payload1.get_motor1());
    EXPECT_EQ(600, payload1.get_motor2());
    EXPECT_EQ(500, payload1.get_motor3());
    EXPECT_EQ(600, payload1.get_motor4());
    EXPECT_EQ(true, event1->hasPayload());
    EXPECT_EQ(0b01010101, payload1.get_in());

    EXPECT_EQ(500, payload2.get_motor1());
    EXPECT_EQ(600, payload2.get_motor2());
    EXPECT_EQ(500, payload2.get_motor3());
    EXPECT_EQ(600, payload2.get_motor4());
    EXPECT_EQ(true, event2->hasPayload());
    EXPECT_EQ(0b01010101, payload2.get_in());

    EXPECT_EQ(false, event3->hasPayload());
    EXPECT_EQ(MSPCOMMANDS::MSP_IDENT, event3->getCommand());
}

TEST(TestUiHandler, TestOutBoundEvents) {
    msp_set_motor_hbridge* payload = new  msp_set_motor_hbridge();
    payload->set_motor1(500);
    payload->set_motor2(600);
    payload->set_motor3(700);
    payload->set_motor4(800);
    payload->set_in(0b10101010);

    Event* event = new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN , payload);

    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<RRP_QUEUES> directions = {RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();

    MockExternal external = MockExternal();
    uihandler.init(&external, &environment, state, serializer);

    queue<Event*>* thisQueue = state->getQueues()->getQueue(RRP_QUEUES::USER_INTERFACE);
    thisQueue->emplace(event);

    thread t(&UiHandler::handleEvent, &uihandler, state);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    state->setIsRunning(false);

    while (!t.joinable()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (t.joinable()) {
        t.join();
    }

    json j = json::parse(external._outbound.substr(0, external._outbound.length() - 1));
    EXPECT_EQ(500, j["payload"]["motor1"]);
    EXPECT_EQ(600, j["payload"]["motor2"]);
    EXPECT_EQ(700, j["payload"]["motor3"]);
    EXPECT_EQ(800, j["payload"]["motor4"]);
    EXPECT_EQ(0b10101010, j["payload"]["in"]);
    EXPECT_EQ("MSP_SET_MOTOR_HBRIDGE", j["command"]);
}

// MockExternal2
TEST(TestUiHandler, TestIoNetworkException) {
    UiHandler uihandler = UiHandler();
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    vector<RRP_QUEUES> directions = {RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER};
    Environment environment = EnviromentProcessor::createEnvironment(manifest);
    Environment* env = EnviromentProcessor::createEnvironmentRef(manifest);
    State* state = StateFactory::createState(environment, directions);
    Serializer<json>* serializer = new Jseralizer();
    MockExternal2 external = MockExternal2();

    EXPECT_CALL(external, recv_rr(_, _)).WillOnce(Return(-1));
    uihandler.init(&external, env, state, serializer);
    Event* event = new Event(MSPCOMMANDS::MSP_NONE, MSPDIRECTION::EXTERNAL_IN);
    queue<Event*>*  q = state->getQueues()->getQueue(RRP_QUEUES::USER_INTERFACE);
    q->push(event);
    
    EXPECT_THROW({uihandler.produce(state);}, NetworkIOException);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
