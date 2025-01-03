#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fatcnt/events/catagorizer.hpp>
#include <fatcnt/events/statushandler.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/protocols/common/mspcommands.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

class MockStatusProcessor : public StateManagerIface {
    MOCK_METHOD(RR_CMODES, getMode, (), (override));
    MOCK_METHOD(RRP_STATUS, getStatus, (), (override));
    MOCK_METHOD(void, addHandler, (EventHandler*), (override));
    MOCK_METHOD(vector<EventHandler*>, getHandlers, (), (override));
    MOCK_METHOD(void, setMode, (RR_CMODES), (override));
    MOCK_METHOD(StateIface*, getState, (), (override));
    MOCK_METHOD(Environment*, getEnv, (), (override));
    MOCK_METHOD(int32_t, getFlags, (), (override));
};

// Mock classes
class MockRrCatagorizerMapper : public RrCatagorizerMapper {
    public:
    MOCK_METHOD(RRP_QUEUES, mapQueue, (Event*), (override));

    void init(Environment* env, StateIface* state, StateManagerIface* statusProcessor) {
        _state = state;
        _env = env;
        _mockStatusProcessor = statusProcessor;
    }

    vector<EventHandler*>  createEventHandlers() override {
        vector<EventHandler*> handlers = {};
        RrStatusHandler* statusHandler = new RrStatusHandler();
        handlers.push_back(statusHandler);
        statusHandler->init(_state, _env, _mockStatusProcessor);
        return handlers;        
    }

    vector<RRP_QUEUES> queueNames() override {
        vector<RRP_QUEUES> names = {};
        return names;
    }

     StateIface* _state = nullptr;
     Environment* _env = nullptr;
     StateManagerIface* _mockStatusProcessor = nullptr;
};


// Test runner
class TestCatagorizer : public ::testing::Test {
   protected:

    // RrQueues*   queues = new Rrqueues(100, chrono::milliseconds(500), chrono::milliseconds(500));
    StateIface* state = nullptr;
    MockRrCatagorizerMapper* mapper = new MockRrCatagorizerMapper;
    RrCatagorizer* catagorizer = new RrCatagorizer();
    Environment* env = nullptr;
    thread* thread_c;
    MockStatusProcessor* _mockStatusProcessor = new MockStatusProcessor();

    void SetUp() override {
        // Setup code
        RrCatagorizer* catagorizer = new RrCatagorizer();
        vector<RRP_QUEUES> queuesNames = {
            RRP_QUEUES::CATEGORIZER, 
            RRP_QUEUES::USER_INTERFACE, 
            RRP_QUEUES::MICROCONTROLLER,
            RRP_QUEUES::STATUS,};

        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        env = EnviromentProcessor::createEnvironmentRef(manifest);
        state = StateFactory::createState(*env, queuesNames);
        mapper = new MockRrCatagorizerMapper();
        mapper->init(env, state, _mockStatusProcessor);
        catagorizer->init(state, env, mapper, _mockStatusProcessor);
    }

    void TearDown() override {
        // Teardown code
        delete(mapper);
        delete(_mockStatusProcessor);
    }
};

// Tests
TEST_F(TestCatagorizer, TestInit) {
    catagorizer->init(state, env, mapper, _mockStatusProcessor);
}

TEST_F(TestCatagorizer, TestIdent) {
    EXPECT_CALL(*mapper, mapQueue(testing::_))
        .Times(testing::AtLeast(1))
        .WillOnce(testing::Return(RRP_QUEUES::STATUS));

    queue<Event*>* queue = state->getQueues()->getQueue(RRP_QUEUES::CATEGORIZER);
    mutex* mtx = state->getQueues()->getLock(RRP_QUEUES::CATEGORIZER);
    const std::lock_guard<std::mutex> lock(*mtx);

    // place an MSP_IDENT request and see that we get something back.
    Event* identEvent = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_IN);
    queue->push(identEvent);
    EXPECT_EQ(1, queue->size());
    mtx->unlock();

    catagorizer->init(state, env, mapper, _mockStatusProcessor);
    std::thread* t = new std::thread(RrCatagorizer::handleEvent, catagorizer, state);
    this_thread::sleep_for(chrono::milliseconds(100));
    
    while(!t->joinable()) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    EXPECT_EQ(0, queue->size());


    queue = state->getQueues()->getQueue(RRP_QUEUES::USER_INTERFACE);
    EXPECT_EQ(1, queue->size());
    identEvent = queue->front();
    EXPECT_EQ(MSPDIRECTION::EXTERNAL_OUT, identEvent->getDirection());
    EXPECT_EQ(MSPCOMMANDS::MSP_IDENT, identEvent->getCommand());
    msp_ident mspIdent = identEvent->getPayload<msp_ident>();
    EXPECT_EQ(LANDDRONE_4W, mspIdent.get_multitype());
    EXPECT_EQ(VIRTUAL, mspIdent.get_msp_version());
    EXPECT_EQ(0, mspIdent.get_capability());

    state->setIsRunning(false);
    t->join();
}

// Main class
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
