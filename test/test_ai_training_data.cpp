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

    uint64_t idx = 1, end_idx = idx;
    ai_data.open_read();

    // starting at index 1, retieve next two rows, this creates the mini batch

    std::vector<std::vector<uint8_t>> tdata;
    std::vector<std::vector<uint8_t>> ldata;
    ai_data.retrieve_data(0, 1, tdata, ldata);
    ai_data.close_read();

}