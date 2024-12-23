#include "statushandler.hpp"

using namespace rrobot;


dlib::logger dlog_st("status_handler");

void RrStatusHandler::init(StateIface *state, Environment *environment, vector<EventHandler*> handlers) {
    dlog_st << dlib::LINFO << "initalizing status handler";
    _environment = environment;
    _handlers = handlers;
    _state = state;
    EventHandler::init(state->getQueues(), RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE, environment);
}

bool RrStatusHandler::isArmed() {
    int32_t flags = getFlags();
    return (flags & DRONE_ARMED) == 0;
}

int32_t RrStatusHandler::getStatus() {
    int32_t flags = getFlags();

    if ((flags & SHUTTING_DOWN) == SHUTTING_DOWN) {
        return SHUTTING_DOWN;
    }
    else if ((flags & ERROR) == ERROR) {
        return ERROR;
    }
    else if ((flags & TERMINATED) == TERMINATED) {
        return TERMINATED;
    }
        else if ((flags & RELOADING) == RELOADING) {
        return RELOADING;
    }
    else if ((flags & INITILIZING) == INITILIZING) {
        return INITILIZING;
    }
    else if ((flags & ACTIVE) == ACTIVE) {
        return ACTIVE;
    }

    return INITILIZING;
}

/**
 * iterate through each handler,  and set the corresponding bit value. If values are anything but
 * 2, then not all components have initialized. For each iteration of flags is set to '0' therefore
 * it should be seen as the current state.
 */
int32_t RrStatusHandler::getFlags() {
    if (_state->isRunning() == false) {
        return SHUTTING_DOWN;
    }

    int32_t flags = 0;
    for (auto handler : _handlers) {
        flags = flags | handler->status();
    }
    return flags;
 }

Event* RrStatusHandler::produce(StateIface* state) {
    void* payload = nullptr;
    Environment env = *_environment;

    switch(_request->getCommand()) {
        case MSP_IDENT:
            {
                msp_ident* mspIdent = new msp_ident();
                mspIdent->set_version(env.getVersion().getVersonMajor());
                mspIdent->set_multitype(env.getHwModel().getMultiType());
                mspIdent->set_msp_version(env.getHwModel().getMspVersion());
                mspIdent->set_capability(0);
                payload = static_cast<void *>(mspIdent);
            }
            break;
        case MSP_STATUS:
            {
                msp_status* mspStatus = new msp_status();
                mspStatus->set_cycletime(state->getCycleTime());
                mspStatus->set_i2c_errors_count(state->getErrorCount());
                mspStatus->set_sensor(state->getSensors());
                mspStatus->set_flag(getFlags());

                // The setting involves ACC_1G which is a current setting,  however this is not 
                // on the drone hardware at the moment, so at the moment this is just set to '0'
                mspStatus->set_current_set(0);
                payload = static_cast<void *>(mspStatus);
            }
            break;
    }

    Event* event;
    if (payload == nullptr) {
        dlog_st << dlib::LERROR << "an unknown request was sent, ignoring";
        msp_error* mspError = new msp_error();
        mspError->set_message("an unknown request was sent.");
        event = new Event(MSPCOMMANDS::MSP_ERROR,  MSPDIRECTION::EXTERNAL_OUT);
    } else {
        event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_OUT, payload);
    }
    dlog_st << dlib::LDEBUG << "sending out status event";
    _request = nullptr;
    return event;
}

bool RrStatusHandler::available() {
    return _request != nullptr;
}

bool RrStatusHandler::consume(Event* event, StateIface* state) {
    _request = event;
    return true;
}