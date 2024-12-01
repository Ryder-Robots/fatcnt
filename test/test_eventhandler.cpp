#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/environment/queues.hpp>
#include <fatcnt/state/rrqueues.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/events/MspAuthKey.hpp>

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
};

class MockState : public StateIface {
    public:
    MOCK_METHOD(bool, isRunning, (), (override));
    MOCK_METHOD(bool, isAuthenticated, (), (override));
};

TEST(TestEventHandler, TestAbstractHandler) {
    MockConcreteHandler* mockConcreteHandler = new MockConcreteHandler();
    RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::CATEGORIZER, q2, lock2);

    mockConcreteHandler->init(rrqueues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    MockState mockState;

    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(mockState, isAuthenticated())
        .WillOnce(Return(true));

    mockConcreteHandler->handleEvent(mockConcreteHandler, &mockState);
}

TEST(TestEventHandler, TestInvalidQueue) {
    MockConcreteHandler* mockConcreteHandler = new MockConcreteHandler();

    RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));

    EXPECT_THROW({
        mockConcreteHandler->init(rrqueues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    }, QueueDoesNotExit);
}


TEST(TestEventHandler, TestConsumeEvent) {
     MockConcreteHandler mockConcreteHandler;
     RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(10), chrono::milliseconds(10));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::CATEGORIZER, q2, lock2);

    mockConcreteHandler.init(rrqueues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    MockState mockState;

    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(mockState, isAuthenticated())
        .WillOnce(Return(true));
    EXPECT_CALL(mockConcreteHandler, consume(_, _))
        .Times(1)
        .WillOnce(Return(true));

    MspAuthKey* payload = new MspAuthKey("test");
    Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::USER_INTERFACE, payload);
    q1->emplace(event);
    mockConcreteHandler.handleEvent(&mockConcreteHandler, &mockState);
}

TEST(TestEventHandler, TestProduceEvent) {
     MockConcreteHandler mockConcreteHandler;
     RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(10), chrono::milliseconds(10));

    mutex* lock1 = new mutex();
    queue<Event*>* q1 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::USER_INTERFACE, q1, lock1);

    mutex* lock2 = new mutex();
    queue<Event*>* q2 = new queue<Event*>();
    rrqueues->setQueue(MSPDIRECTION::CATEGORIZER, q2, lock2);

    mockConcreteHandler.init(rrqueues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    MockState mockState;
    EXPECT_CALL(mockState, isRunning())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(mockState, isAuthenticated())
        .WillOnce(Return(true));

    MspAuthKey* payload = new MspAuthKey("test");
    Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::USER_INTERFACE, payload);
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