#include <statusprocessor.hpp>

RR_CMMODES StatusProcessor::getMode(int32_t flags) {
    return RR_CMODES::CMODE_NOT_SET;
}

RRP_STATUS StatusProcessor::getStatus(int32_t flags) {
    return RRP_STATUS::INITILIZING;
}