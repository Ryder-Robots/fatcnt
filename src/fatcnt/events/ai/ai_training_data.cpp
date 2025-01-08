#include <fatcnt/events/ai/ai_training_data.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

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

void AiGenerateData::generate(std::vector<string> headings, std::vector<uint8_t> training, std::vector<uint8_t> label) {
    ios_base::openmode flags = ios::out;
    
    if (!_outstream_data.is_open()) {
        if (fs::exists(_label_fname)) {
            flags = flags | ios::app;
            _idx = getGenesis();
        } else {
            flags = flags | ios::trunc;
        }
        _outstream_data.open(_data_fname, flags);
        _outstream_labels.open(_label_fname, flags);

        if (_outstream_data.fail() || _outstream_labels.fail()) {
            throw new RrIOException("could not open training data");
        }

        if (flags & ios::trunc) {
            dlog_ai_td << dlib::LINFO << "creating headings";
            headings.insert(headings.begin(), "idx");
            string hdr = boost::algorithm::join(headings, ",");
            hdr += "\n";
            _outstream_data.write(hdr.c_str(), static_cast<std::streamsize>(hdr.size()));
            _outstream_labels.write(hdr.c_str(), static_cast<std::streamsize>(hdr.size()));
        }
    }
    dlog_ai_td << dlib::LINFO << "adding data:" + _idx;
    write_ai_data(training, &_outstream_data);
    write_ai_data(label, &_outstream_labels);
    _idx++;
}

void AiGenerateData::write_ai_data(std::vector<uint8_t> v, ofstream* s) {
    std::vector<string> tt;
    tt.insert(tt.begin(), std::to_string(_idx));
    for (const auto& byte : v) {
        tt.push_back(std::to_string(static_cast<int>(byte)));
    }
    string d = boost::algorithm::join(tt, ",") + "\n";
    s->write(d.c_str(), static_cast<std::streamsize>(d.size()));
}


uint64_t AiGenerateData::getGenesis() {
    if (fs::exists(_label_fname)) {
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