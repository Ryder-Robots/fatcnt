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
    curator->deserialize(inbound);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}