#include "statusprocessor.hpp"

using namespace rrobot;

RR_CMODES StatusProcessor::getMode() {
    int32_t flags = _state->getFlags();
    flags -= (flags & INITILIZING) + (flags & ERROR) + (flags & RELOADING) + 
        (flags + SHUTTING_DOWN) + (flags & TERMINATED) + (flags & ACTIVE);
    return static_cast<RR_CMODES>(flags);
}

RRP_STATUS StatusProcessor::getStatus() {
    int32_t statuses = 0;
    for (auto handler : _eventHandlers) {
        statuses = statuses | handler->status();
    }
    int32_t flags_modes = _state->getFlags() & CMODE_MANUAL_FLIGHT + _state->getFlags() & CMODE_NOT_SET;

    RRP_STATUS status = RRP_STATUS::INITILIZING;
    vector<RRP_STATUS> statusArray = {TERMINATED, SHUTTING_DOWN, ERROR, RELOADING, INITILIZING, ACTIVE};

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
    int32_t statuses = (flags & INITILIZING) + (flags & ERROR) + (flags & RELOADING) + 
        (flags + SHUTTING_DOWN) + (flags & TERMINATED) + (flags & ACTIVE);    
    _state->setFlags(0 | statuses | mode);
}