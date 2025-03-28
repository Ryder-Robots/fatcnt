#include <fatcnt/protocols/rmultiwii/rmultiwii.hpp>

using namespace rrobot;

dlib::logger dlog_mw("rr_curator");

RmMultiWii RmMultiWii::createInstance(uint8_t* data, Crc32 crc, Environment* environment) {
    dlog_mw.set_level(environment->getLogging().getLogLevel());
    try {
        MSPCOMMANDS cmd = getSupportCommand(data[0]);
        int16_t sz = static_cast<uint16_t>(data[1] | (data[2] << 8));
        int32_t crc32 = static_cast<uint32_t>(data[3] | data[4] << 8 | data[5] << 16 | data[6] << 24);

        uint8_t* payload = new uint8_t[sz];
        for (int i = 0; i < sz; i++) {
            payload[i] = data[i + 7];

            if (data[i + 7] == _TERMINATION_CHAR) {
                throw RrIOException("unexpected character in sequence");
            }
        }

        RmMultiWii result = RmMultiWii(cmd, sz);
        if (sz > 0) {
            if (crc32 != crc.calculate(payload, sz)) {
                throw RrIOException("failed CRC check");
            }
            result.setPayload(payload);
        }
        return result;
    } catch (const std::out_of_range& e) {
        dlog_mw << dlib::LERROR << "could not create instance - out of bounds error recieved : " << e.what();
        return RmMultiWii(MSPCOMMANDS::MSP_ERROR, 0);
    } catch (const RrIOException& e) {
        dlog_mw << dlib::LERROR << "could not create instance - unexpected input recieved : " << e.what();
        return RmMultiWii(MSPCOMMANDS::MSP_ERROR, 0);
    }
}

uint8_t* RmMultiWii::encode(Crc32 crc) {
    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(sizeof(char) * 8 + sizeof(char) * _sz));
    data[0] = static_cast<uint8_t>(_cmd);
    
    // size
    data[1] = (_sz >> 8) & 0xFF;
    data[2] = _sz & 0xFF;

    // calculate CRC
    uint32_t crc32 = 0;

    if (_sz != 0) {
        crc32 = crc.calculate(getPayload(), _sz);
    }
    data[3] = (_sz >> 24) & 0xFF;
    data[4] = (_sz >> 16) & 0xFF;
    data[5] = (_sz >> 8) & 0xFF;
    data[6] = _sz & 0xFF;

    if (_sz != 0) {
        for (int i = 0;i < _sz; i++) 
            data[7 + i] = _payload[i];
    }
    data[7 + _sz] = _TERMINATION_CHAR & 0xFF;
    return data; 
}

RmMultiWii::~RmMultiWii() {
    if (_sz > 0) free(_payload);
}