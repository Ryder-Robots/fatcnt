#include <gtest/gtest.h>

#include <fatcnt/events/ui/jserializer.hpp>
#include <fatcnt/state/rrpqueues.hpp>

using namespace rrobot;

class TestJserializer : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

TEST(TestJserializer, TestSerialize) {
    Jserializer jserializer = Jserializer();

    msp_authkey *mspAuthKey = new msp_authkey();
    mspAuthKey->set_key("test");
    Event *event = new Event(MSPCOMMANDS::MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, mspAuthKey);
    json out = jserializer.serialize(event);

    EXPECT_EQ("MSP_AUTHKEY", out["command"]);
    EXPECT_EQ("test", out["payload"]["key"]);
}

TEST(TestJserializer, TestDeserialize) {
    json in = {
        {"command", "MSP_AUTHKEY"}
    };
    in["payload"]["key"] = "test";

    cout << "json in = " + in.dump() << "\n";

    Jserializer jserializer = Jserializer();
    Event* event = jserializer.deserialize(in);
    msp_authkey payload = event->getPayload<msp_authkey>();

    EXPECT_EQ(MSPCOMMANDS::MSP_AUTHKEY, event->getCommand());
    EXPECT_EQ("test", payload.get_key());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
