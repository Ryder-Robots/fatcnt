#include <fatcnt/events/ai/ai_training_data.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

dlib::logger dlog_ai_td("training_data");

AiGenerateData::AiGenerateData(Environment* env) {
    dlog_ai_td.set_level(env->getLogging().getLogLevel());
    dlog_ai_td << dlib::LINFO << "initilizing AI Training Data";
    _data_fname = env->getAuTrainingData().getData_file();
}

AiGenerateData::~AiGenerateData() {}

void AiGenerateData::open_write() {
    dlog_ai_td << dlib::LINFO << "opening training data files";
    _outstream_data.open(_data_fname, ios::app | ios::binary);

    if (!_outstream_data.is_open()) {
        dlog_ai_td << dlib::LFATAL << "could not create training files";
        throw new RrIOException("could not create training files");
    }
}

void AiGenerateData::open_read() {
    dlog_ai_td << dlib::LINFO << "opening training data files for read";
    _instream_data.open(_data_fname, ios::binary);

    if (!_instream_data.is_open()) {
        dlog_ai_td << dlib::LFATAL << "could not open training files for read";
        throw new RrIOException("could not read training files for read");
    }
}

void AiGenerateData::close_write() {
    if (_outstream_data.is_open()) {
        _outstream_data.close();
    }

}

void AiGenerateData::close_read() {
    if (_instream_data.is_open()) {
        _instream_data.close();
    }
}

size_t AiGenerateData::write_data(std::vector<uint8_t> training, std::vector<uint8_t> label) {
    size_t ts = training.size();
    size_t ls = label.size();

    _outstream_data.write(reinterpret_cast<char*>(&ts), sizeof(ts));
    _outstream_data.write(reinterpret_cast<char*>(&ls), sizeof(ls));
    _outstream_data.write(reinterpret_cast<char*>(training.data()), training.size() * sizeof(uint8_t));
    _outstream_data.write(reinterpret_cast<char*>(label.data()), label.size() * sizeof(uint8_t));
    return training.size() * sizeof(uint8_t) + label.size() * sizeof(uint8_t);
}

streampos AiGenerateData::retrieve_data(streampos idx, size_t count, 
std::vector<std::vector<uint8_t>>& training,  
std::vector<std::vector<uint8_t>>& labels) {
    std::vector<uint8_t> vtrain, vlabels;
    size_t ts, ls;
    _instream_data.seekg(idx);

    for (int i = 0; i < count; i++) {
        if (_instream_data.eof() == true) {
            break;
        }

        _instream_data.read(reinterpret_cast<char*>(&ts), sizeof(ts));
        _instream_data.read(reinterpret_cast<char*>(&ls), sizeof(ls));
        vtrain.resize(ts);
        vlabels.resize(ls);
        _instream_data.read(reinterpret_cast<char*>(vtrain.data()), ts * sizeof(uint8_t));
        _instream_data.read(reinterpret_cast<char*>(vlabels.data()), ls * sizeof(uint8_t));

        training.push_back(vtrain);
        labels.push_back(vlabels);
    }
    return _instream_data.tellg();
}
