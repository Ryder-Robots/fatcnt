#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/environment/queues.hpp>
#include <fatcnt/state/rrqueues.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_authkey.hpp>

using namespace rrobot;

using ::testing::Return;
using ::testing::_;

class TestEventHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

class MockConcreteHandler : public EventHandler {
    public:
    MOCK_METHOD(bool, available, (), (override));
    MOCK_METHOD(bool, consume, (Event*, StateIface*), (override));
    MOCK_METHOD(Event*, produce, (StateIface*), (override));

    RRP_STATUS status() {
        return RRP_STATUS::ACTIVE;
    }

    string name() override {return "mockhandler";}
};

class MockState : public StateIface {
    public:
    MOCK_METHOD(bool, isRunning, (), (override));
    MOCK_METHOD(uint16_t, getCycleTime, (), (override));
    MOCK_METHOD(uint16_t, getErrorCount, (), (override));
    MOCK_METHOD(uint16_t, getSensors, (), (override));
    MOCK_METHOD(RR_CMMODES, getMode, (), (override));
    void setIsRunning(bool isrunning) override {}
    RrQueues* getQueues() override {return nullptr;}
    void setCycleTime(uint16_t cycleTime) override {}
    void incremementErrorCount() override {}
    void setSensorFlag(MSPSENSOR_FLAGS flag) override {}
    void setMode(RR_CMMODES mode) override {}

};

TEST(TestEventHandler, TestAbstractHandler) {
    MockConcreteHandler* mockConcreteHandler = new MockConcreteHandler();
    RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::CATEGORIZER, q2, lock2);

    mockConcreteHandler->init(rrqueues, RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER);
    MockState mockState;

    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    mockConcreteHandler->handleEvent(mockConcreteHandler, &mockState);
}

TEST(TestEventHandler, TestInvalidQueue) {
    MockConcreteHandler* mockConcreteHandler = new MockConcreteHandler();

    RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));

    EXPECT_THROW({
        mockConcreteHandler->init(rrqueues, RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER);
    }, QueueDoesNotExit);
}


TEST(TestEventHandler, TestConsumeEvent) {
     MockConcreteHandler mockConcreteHandler;
     RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(10), chrono::milliseconds(10));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::CATEGORIZER, q2, lock2);

    mockConcreteHandler.init(rrqueues, RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER);
    MockState mockState;

    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(mockConcreteHandler, consume(_, _))
        .Times(1)
        .WillOnce(Return(true));

    msp_authkey* payload = new msp_authkey();
    payload->set_key("test");
    Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, payload);
    q1->emplace(event);
    mockConcreteHandler.handleEvent(&mockConcreteHandler, &mockState);
}

TEST(TestEventHandler, TestProduceEvent) {
     MockConcreteHandler mockConcreteHandler;
     RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(10), chrono::milliseconds(10));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(RRP_QUEUES::CATEGORIZER, q2, lock2);

    mockConcreteHandler.init(rrqueues, RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER);
    MockState mockState;
    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    msp_authkey* payload = new msp_authkey();
    payload->set_key("test");
    Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, payload);
    EXPECT_CALL(mockConcreteHandler, produce(_))
        .Times(1)
        .WillOnce(Return(event));
    EXPECT_CALL(mockConcreteHandler, available())
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    mockConcreteHandler.handleEvent(&mockConcreteHandler, &mockState);
    EXPECT_EQ(1, q2->size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}