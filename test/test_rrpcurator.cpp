#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fatcnt/protocols/common/curators/rrp/mspevent.hpp>

using namespace rrobot;

using ::testing::Return;
using ::testing::_;

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}