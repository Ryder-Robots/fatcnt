#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/environment/queues.hpp>
#include <fatcnt/state/rrqueues.hpp>
#include <fatcnt/state/stateiface.hpp>

using namespace rrobot;

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
    MOCK_METHOD(bool, available, (), (override));
    MOCK_METHOD(void, consume, (Event, StateIface*), (override));
    MOCK_METHOD(Event, produce, (StateIface*), (override));
};

TEST(TestEventHandler, TestAbstractHandler) {
    MockConcreteHandler* mockConcreteHandler = new MockConcreteHandler();

    RrQueues* rrqueues = new RrQueues(100, chrono::milliseconds(500), chrono::milliseconds(500));


    mockConcreteHandler->init(rrqueues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    // EXPECT_CALL(*MockConcreteHandler, available());
}