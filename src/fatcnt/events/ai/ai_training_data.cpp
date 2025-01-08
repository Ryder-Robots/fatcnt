#include <ai_training_data.hpp>

using namespace rrobot;

dlib::logger dlog_ai_td("training_data");

AiGenerateData::AiGenerateData(Environment *env) {
    dlog_ai_td.set_level(env->getLogging().getLogLevel());
}

void AiGenerateData::generate(string trainingSet, std::vector<uint8_t> training, std::vector<uint8_t> label) {

}

