#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fatcnt/events/catagorizer.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>
#include <fatcnt/protocols/common/mspcommands.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

// Mock classes
class MockRrCatagorizerMapper : public RrCatagorizerMapper {

};


// Test runner
class TestCatagorizer : public ::testing::Test {
   protected:

    RrQueues*   queues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));
    StateIface* state = nullptr;
    MockRrCatagorizerMapper* mapper = new MockRrCatagorizerMapper;
    RrCatagorizer* catagorizer = new RrCatagorizer();
    Environment* env = nullptr;
    thread* thread_c;

    void SetUp() override {
        // Setup code
        RrCatagorizer* catagorizer = new RrCatagorizer();
        vector<RRP_QUEUES> queuesNames = {RRP_QUEUES::CATEGORIZER, RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::MICROCONTROLLER,};

        for (auto queueName : queuesNames) {
            mutex* lock = new mutex();
            queue<Event*>* q = new queue<Event*>();
            queues->setQueue(queueName, q, lock);
        }

        const fs::path filepath = "manifests/virtual.json";
        ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        env = EnviromentProcessor::createEnvironmentRef(manifest);
        state = StateFactory::createState(*env, queuesNames);
        mapper = new MockRrCatagorizerMapper();

        catagorizer->init(queues, state, env, mapper);
    }

    void TearDown() override {
        // Teardown code
    }
};

// Tests
TEST_F(TestCatagorizer, TestInit) {
    catagorizer->init(queues, state, env, mapper);
}

TEST_F(TestCatagorizer, TestIdent) {
    queue<Event*>* queue = queues->getQueue(RRP_QUEUES::CATEGORIZER);
    mutex* mtx = queues->getLock(RRP_QUEUES::CATEGORIZER);
    const std::lock_guard<std::mutex> lock(*mtx);

    // place an MSP_IDENT request and see that we get something back.
    Event* identEvent = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_IN);
    queue->push(identEvent);
    EXPECT_EQ(1, queue->size());
    mtx->unlock();

    catagorizer->init(queues, state, env, mapper);
    std::thread* t = new std::thread(RrCatagorizer::handleEvent, catagorizer, state);
    this_thread::sleep_for(chrono::milliseconds(10));
    
    while(!t->joinable()) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    state->setIsRunning(false);
    t->join();

    EXPECT_EQ(0, queue->size());

    queue = queues->getQueue(RRP_QUEUES::USER_INTERFACE);
    EXPECT_EQ(1, queue->size());
    identEvent = queue->front();
    EXPECT_EQ(MSPDIRECTION::EXTERNAL_OUT, identEvent->getDirection());
    EXPECT_EQ(MSPCOMMANDS::MSP_IDENT, identEvent->getCommand());
    msp_ident mspIdent = identEvent->getPayload<msp_ident>();
    EXPECT_EQ(LANDDRONE_4W, mspIdent.get_multitype());
    EXPECT_EQ(VIRTUAL, mspIdent.get_msp_version());
    EXPECT_EQ(0, mspIdent.get_capability());
}

// Main class
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
