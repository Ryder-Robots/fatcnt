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
        const fs::path filepath = "manifests/virtual.json";
        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();
        Environment env = EnviromentProcessor::createEnvironment(manifest);

        _ai_data = new AiGenerateData(&env);
    }
    
    void TearDown() override {
        // Teardown code
        delete(_ai_data);
    }

    AiGenerateData* _ai_data;
};

TEST_F(TestAiTrainingData, shouldGetGenesis) {
    uint64_t idx = _ai_data->getGenesis();

    EXPECT_EQ(100, idx);
}