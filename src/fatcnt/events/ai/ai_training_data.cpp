#include <fatcnt/events/ai/ai_training_data.hpp>

using namespace rrobot;

dlib::logger dlog_ai_td("training_data");

AiGenerateData::AiGenerateData(Environment *env) {
    dlog_ai_td << dlib::LINFO << "initilizing AI Training Data";
    dlog_ai_td.set_level(env->getLogging().getLogLevel());

    _data_fname = env->getAuTrainingData().getData_file();
    _label_fname = env->getAuTrainingData().getLabel_file();
}

AiGenerateData::~AiGenerateData() {
    dlog_ai_td << dlib::LINFO << "closing training set";
    if (_outstream_labels.is_open()) {
         _outstream_labels.close();
    }
    if (_outstream_data.is_open()) {
        _outstream_data.close();
    }
    dlog_ai_td << dlib::LINFO << "closing training set closed";
   
}

void AiGenerateData::generate(std::vector<uint8_t> training, std::vector<uint8_t> label) {
}


uint64_t AiGenerateData::getGenesis() {
    if (std::filesystem::exists(_label_fname)) {
        dlog_ai_td << dlib::LINFO << "found existing labels file";

        lazycsv::parser parser{_label_fname};
        auto first_row = *parser.begin();
        const auto [idx] = first_row.cells(0);
        string idx_s = string(idx.trimed());
        _idx = std::strtoul(idx_s.c_str(), nullptr, 10);
    }
    return _idx;
}

matrix<std::vector<uint8_t>> AiGenerateData::retrieveTraining(uint64_t* idx, size_t count) {
    matrix<std::vector<uint8_t>> m;
    return m;
}

matrix<std::vector<uint8_t>> AiGenerateData::retrieveLabels(uint64_t start, uint64 end) {
    matrix<std::vector<uint8_t>> m;
    return m;
}