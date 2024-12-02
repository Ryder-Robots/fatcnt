#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <fatcnt/protocols/common/curators/rrp/mspevent.hpp>
#include <fatcnt/protocols/common/curators/rrp/rrpcurator.hpp>

using namespace rrobot;

using ::testing::Return;
using ::testing::_;

using json = nlohmann::json;

class TestRrpCurator : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};


TEST(TestRrpCurator, TestGeneratedEvents) {
    msp_ident* payload = new msp_ident();
    payload->set_msp_version(MSP_VERSION::SKULD002);
    payload->set_multitype(MULTITYPE_T::QUADP);
    payload->set_version(1);
    payload->set_capability(2);

    EXPECT_EQ(MSP_VERSION::SKULD002, payload->get_msp_version());
    EXPECT_EQ(MULTITYPE_T::QUADP, payload->get_multitype());
    EXPECT_EQ(1, payload->get_version());
    EXPECT_EQ(2, payload->get_capability());
}

TEST(TestRrpCurator, TestSingleCurator) {
    // This is not an inbound event,  but a good test because it uses custom types
    json inbound = {
        {"msp_version", "SKULD002"},
        {"multitype", "QUADP"},
        {"version", 1},
        {"capability", 2}
    };

    msp_ident_curator *curator = new msp_ident_curator();
    Event* event = curator->deserialize(inbound);
    msp_ident payload = event->getPayload<msp_ident>();

    EXPECT_EQ(MSP_VERSION::SKULD002, payload.get_msp_version());
    EXPECT_EQ(MULTITYPE_T::QUADP, payload.get_multitype());
    EXPECT_EQ(1, payload.get_version());
    EXPECT_EQ(2, payload.get_capability());   
}

TEST(TestRrpCurator, TestSerialize) {
    msp_ident *payload = new msp_ident();
    payload->set_msp_version(MSP_VERSION::SKULD002);
    payload->set_multitype(MULTITYPE_T::QUADP);
    payload->set_version(1);
    payload->set_capability(2);
    Event* event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::USER_INTERFACE, payload);

    msp_ident_curator *curator = new msp_ident_curator();
    json outbound = curator->serialize(event);

    EXPECT_EQ("MSP_IDENT", outbound["command"]);
    EXPECT_EQ("SKULD002", outbound["payload"]["msp_version"]);
    EXPECT_EQ("QUADP", outbound["payload"]["multitype"]);
    EXPECT_EQ(1, outbound["payload"]["version"]);
    EXPECT_EQ(2, outbound["payload"]["capability"]);       
}

TEST(TestRrpCurator, TestGeneratedDeserialize) {
    json inbound = {"key", "test"};
    msp_authkey_curator *curator = new msp_authkey_curator();
    Event* event = curator->deserialize(inbound);

    EXPECT_EQ(MSPCOMMANDS::MSP_AUTHKEY, event->getCommand());
    EXPECT_EQ(MSPDIRECTION::USER_INTERFACE, event->getDirection());

    msp_authkey payload = event->getPayload<msp_authkey>();
    EXPECT_EQ("test", payload.get_key());
}

TEST(TestRrpCurator, TestGeneratedSerialize) {
    msp_authkey *payload = new msp_authkey();
    payload->set_key("test");
    Event* event = new Event(MSPCOMMANDS::MSP_AUTHKEY, MSPDIRECTION::USER_INTERFACE, payload);

    msp_authkey_curator *curator = new msp_authkey_curator();
    json outbound = curator->serialize(event);

    EXPECT_EQ("MSP_AUTHKEY", outbound["command"]);
    EXPECT_EQ("test", outbound["payload"]["key"]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}