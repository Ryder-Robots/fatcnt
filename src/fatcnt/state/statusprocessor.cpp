#include "statusprocessor.hpp"

using namespace rrobot;

RR_CMODES StatusProcessor::getMode() {
    int32_t flags = _state->getFlags();
    flags -= STATUS_BITMASK(flags);
    return static_cast<RR_CMODES>(flags);
}

RRP_STATUS StatusProcessor::getStatus() {
    int32_t statuses = 0;
    for (auto handler : _eventHandlers) {
        statuses = statuses | handler->status();
    }
    int32_t flags_modes = MODE_BITMASK(_state->getFlags());

    RRP_STATUS status = RRP_STATUS::INITILIZING;
    vector<RRP_STATUS> statusArray = STATUS_ARRAY_INIT;

    // This is the a precise order on what global status will be.
    for (auto s : statusArray) {
        if ((statuses & s) == s) {
            status = s;
            break;
        }
    }
    int32_t flags = statuses + flags_modes;
    _state->setFlags(flags);
    return status;
}

void StatusProcessor::setMode(RR_CMODES mode) {
    int32_t flags = _state->getFlags();
    int32_t statuses = STATUS_BITMASK(flags);    
    _state->setFlags(0 | statuses | mode);
}