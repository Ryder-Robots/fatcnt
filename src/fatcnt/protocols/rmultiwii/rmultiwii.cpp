#include <fatcnt/protocols/rmultiwii/rmultiwii.hpp>

using namespace rrobot;

RmMultiWii RmMultiWii::createInstance(uint8_t* data, Crc32 crc) {
    return RmMultiWii(MSPCOMMANDS::MSP_SENSOR, 0);
}


uint8_t* RmMultiWii::encode(Crc32 crc) {
    return nullptr;
}