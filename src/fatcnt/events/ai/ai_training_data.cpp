#include <fatcnt/events/ai/ai_training_data.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

dlib::logger dlog_ai_td("training_data");

AiGenerateData::AiGenerateData(Environment* env) {
    dlog_ai_td << dlib::LINFO << "initilizing AI Training Data";
    dlog_ai_td.set_level(env->getLogging().getLogLevel());

    _data_fname = env->getAuTrainingData().getData_file();
    _label_fname = env->getAuTrainingData().getLabel_file();
    _idx_position.reserve(4);
    dlog_ai_td << dlib::LINFO << "reserving " << _idx_position.size() << "for indices";
}

AiGenerateData::~AiGenerateData() {}

void AiGenerateData::open_write() {
    dlog_ai_td << dlib::LINFO << "Opening training data files";
    _outstream_data.open(_data_fname, ios::app | ios::binary);
    _outstream_labels.open(_label_fname, ios::app | ios::binary);

    if (!(_outstream_data.is_open() && _outstream_labels.is_open())) {
        throw new RrIOException("could not create training files");
    }
}

void AiGenerateData::open_read() {
    _instream_data.open(_data_fname, ios::binary);
    _instream_labels.open(_label_fname, ios::binary);

    if (!(_instream_data.is_open() && _instream_labels.is_open())) {
        throw new RrIOException("could not read training files");
    }
}

void AiGenerateData::close_write() {
    if (_outstream_data.is_open()) {
        _outstream_data.close();
    }
    if (_outstream_labels.is_open()) {
        _outstream_labels.close();
    }
}

void AiGenerateData::close_read() {
    if (_instream_data.is_open()) {
        _instream_data.close();
    }

    if (_instream_labels.is_open()) {
        _instream_labels.close();
    }
}

size_t AiGenerateData::write_data(std::vector<uint8_t> training, std::vector<uint8_t> label) {
    _idx_position.clear();
    _idx_position.push_back(_idx);
    _idx_position.push_back(_outstream_data.tellp());
    _outstream_data.write(reinterpret_cast<char*>(training.data()), training.size() * sizeof(uint8_t));
    _idx_position.push_back(_outstream_data.tellp());

    _outstream_data.write(reinterpret_cast<char*>(label.data()), label.size() * sizeof(uint8_t));
    _idx_position.push_back(_outstream_data.tellp());

    _outstream_labels.write(reinterpret_cast<const char*>(_idx_position.data()), _idx_position.size() * sizeof(uint64_t));
    _idx++;

    return training.size() * sizeof(uint8_t) + label.size() * sizeof(uint8_t);
}

uint64_t AiGenerateData::retrieve_data(uint64_t idx, size_t count, const matrix<std::vector<uint8_t>>& training,
                                       const matrix<std::vector<uint8_t>>& labels) {
    // compute the size of the file.
    bool found = false;
    _instream_labels.seekg(0, std::ios::end);
    streampos size = _instream_labels.tellg(), relpos = size - IDX_POS_SZ;
    _instream_labels.read(reinterpret_cast<char*>(_idx_position.data()), IDX_POS_SZ);

    if (_idx_position[0] == idx) {
        // last record happens to be correct one, and can't go any further.
        return _idx_position[0];
    }
    // reset to start of file
    _instream_labels.seekg(0, std::ios::beg);
    _instream_labels.read(reinterpret_cast<char*>(_idx_position.data()), IDX_POS_SZ);

    uint64_t x = _idx_position[0];

    if (_idx_position[0] != idx) {
        // find starting point
        int high = size / IDX_POS_SZ, low = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            _instream_labels.seekg(mid * IDX_POS_SZ);
            _instream_labels.read(reinterpret_cast<char*>(_idx_position.data()), IDX_POS_SZ);
            if (_idx_position[0] == idx) {
                found = true;
                break;
            } else if (_idx_position[0] < idx) {
                low = mid + 1;  // Search in right half
            } else {
                high = mid - 1;  // Search in left half
            }
        }
    } else {
        found = true;
    }

    return 0;
}
