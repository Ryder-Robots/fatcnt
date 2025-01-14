#include <gtest/gtest.h>
#include <filesystem>
#include <iostream>
#include <fatcnt/events/ai/ai_training_data.hpp>
#include <fatcnt/environment/environmentProcessor.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

class TestAiTrainingData : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
        const fs::path filepath = "manifests/virtual.2.json";
        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();
        Environment env = EnviromentProcessor::createEnvironment(manifest);

        _ai_data = new AiGenerateData(&env);

        // renmove test files
        remove("var/tset.2.trn.data");
        remove("var/tset.2.lbl.data");
    }
    
    void TearDown() override {
        // Teardown code
        delete(_ai_data);
    }

    AiGenerateData* _ai_data;
};


TEST_F(TestAiTrainingData,  shouldGenerateData) {
    // std::vector<string> t_headings = {"roll", "pitch", "yaw", "throttle", "aux1", "aux2", "aux3", "aux4"};
    // std::vector<string> l_headings = {"in", "motor1", "motor2", "motor3", "motor4"};


    const fs::path filepath = "manifests/virtual.2.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();
    Environment env = EnviromentProcessor::createEnvironment(manifest);

    AiGenerateData ai_data(&env);

    std::vector<uint8_t> training_data1 = {128,127,230,245,127,127,127,127};
    std::vector<uint8_t> training_data2 = {127,127,225,244,127,127,127,127};
    std::vector<uint8_t> training_data3 = {127,127,220,243,127,127,127,127};
    std::vector<uint8_t> label_data = {5,230,230,0,0};

    ai_data.open_write();

    ai_data.write_data(training_data1, label_data);
    ai_data.write_data(training_data2, label_data);
    ai_data.write_data(training_data3, label_data);

    ai_data.close_write();

    ai_data.open_read();
    std::vector<std::vector<uint8_t>> tdata;
    std::vector<std::vector<uint8_t>> ldata;
    streampos idx = ai_data.retrieve_data(0, 1, tdata, ldata);
   
    EXPECT_EQ(128, tdata[0][0]);
    EXPECT_EQ(127, tdata[0][1]);
    EXPECT_EQ(230, tdata[0][2]);
    EXPECT_EQ(245, tdata[0][3]);
    EXPECT_EQ(127, tdata[0][4]);
    EXPECT_EQ(127, tdata[0][5]);
    EXPECT_EQ(127, tdata[0][6]);
    EXPECT_EQ(127, tdata[0][7]);

    EXPECT_EQ(5, ldata[0][0]);
    EXPECT_EQ(230, ldata[0][1]);
    EXPECT_EQ(230, ldata[0][2]);
    EXPECT_EQ(0, ldata[0][3]);
    EXPECT_EQ(0, ldata[0][4]);

    tdata.clear();
    ldata.clear();

    // expect that the rest of the values are retrieved
    idx = ai_data.retrieve_data(idx, 2, tdata, ldata);
    EXPECT_EQ(2, tdata.size());

    //r row 2
    EXPECT_EQ(127, tdata[0][0]);
    EXPECT_EQ(127, tdata[0][1]);
    EXPECT_EQ(225, tdata[0][2]);
    EXPECT_EQ(244, tdata[0][3]);
    EXPECT_EQ(127, tdata[0][4]);
    EXPECT_EQ(127, tdata[0][5]);
    EXPECT_EQ(127, tdata[0][6]);
    EXPECT_EQ(127, tdata[0][7]);

    EXPECT_EQ(5, ldata[0][0]);
    EXPECT_EQ(230, ldata[0][1]);
    EXPECT_EQ(230, ldata[0][2]);
    EXPECT_EQ(0, ldata[0][3]);
    EXPECT_EQ(0, ldata[0][4]);

    // row 3
    EXPECT_EQ(127, tdata[1][0]);
    EXPECT_EQ(127, tdata[1][1]);
    EXPECT_EQ(220, tdata[1][2]);
    EXPECT_EQ(243, tdata[1][3]);
    EXPECT_EQ(127, tdata[1][4]);
    EXPECT_EQ(127, tdata[1][5]);
    EXPECT_EQ(127, tdata[1][6]);
    EXPECT_EQ(127, tdata[1][7]);

    EXPECT_EQ(5, ldata[1][0]);
    EXPECT_EQ(230, ldata[1][1]);
    EXPECT_EQ(230, ldata[1][2]);
    EXPECT_EQ(0, ldata[1][3]);
    EXPECT_EQ(0, ldata[1][4]);

    ai_data.close_read();
}