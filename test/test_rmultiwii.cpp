#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fatcnt/protocols/rmultiwii/rmultiwii.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>

using namespace rrobot;
using json = nlohmann::json;

namespace fs = std::filesystem;

class TestMultiWii : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
 };

 TEST_F(TestMultiWii, shouldEncodeStatus) {
    const fs::path filepath = "manifests/virtual.json";
    ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();
    Environment* environment = EnviromentProcessor::createEnvironmentRef(manifest);


    Crc32 crc = Crc32();
    uint8_t data[] = {101, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E};
    RmMultiWii multiWii = RmMultiWii::createInstance(data, crc, environment);

    ASSERT_EQ(MSPCOMMANDS::MSP_STATUS, multiWii.getCommand());
    ASSERT_EQ(0, multiWii.getSize());
    
    uint8_t* data2 = multiWii.encode(crc);

    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(data[i], data2[i]);
    }
 }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
