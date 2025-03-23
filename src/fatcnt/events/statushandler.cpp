#include <fatcnt/events/statushandler.hpp>

using namespace rrobot;


dlib::logger dlog_st("status_handler");

void RrStatusHandler::init(StateIface *state, Environment *environment, StateManagerIface* statusProcessor) {
    dlog_st << dlib::LINFO << "initalizing status handler";
    _environment = environment;
    _statusProcessor = statusProcessor;
    _state = state;
    EventHandler::init(state->getQueues(), RRP_QUEUES::STATUS, RRP_QUEUES::USER_INTERFACE, environment);
}


Event* RrStatusHandler::produce(StateIface* state) {
    void* payload = nullptr;
    Environment env = *_environment;
    Event* event = nullptr;

    switch(_request->getCommand()) {
        case MSP_IDENT:
            {
                msp_ident* mspIdent = new msp_ident();
                mspIdent->set_version(env.getVersion().getVersonMajor());
                mspIdent->set_multitype(env.getHwModel().getMultiType());
                mspIdent->set_msp_version(env.getHwModel().getMspVersion());
                mspIdent->set_capability(0);
                payload = static_cast<void *>(mspIdent);
                event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_OUT, payload);
            }
            break;
        case MSP_STATUS:
            {
                // Reset status before processing. In case one of the handlers has malfunctioned for whatever reason.
                _statusProcessor->getStatus();
                msp_status* mspStatus = new msp_status();
                mspStatus->set_cycletime(state->getCycleTime());
                mspStatus->set_i2c_errors_count(state->getErrorCount());
                mspStatus->set_sensor(state->getSensors());
                mspStatus->set_flag(_statusProcessor->getFlags());

                // The setting involves ACC_1G which is a current setting,  however this is not 
                // on the drone hardware at the moment, so at the moment this is just set to '0'
                mspStatus->set_current_set(0);
                payload = static_cast<void *>(mspStatus);
                event = new Event(MSPCOMMANDS::MSP_STATUS, MSPDIRECTION::EXTERNAL_OUT, payload);
            }
            break;
    }

    if (payload == nullptr || event == nullptr) {
        dlog_st << dlib::LERROR << "an unknown request was sent, ignoring";
        msp_error* mspError = new msp_error();
        mspError->set_message("an unknown request was sent.");
        event = new Event(MSPCOMMANDS::MSP_ERROR,  MSPDIRECTION::EXTERNAL_OUT);
    } 
    _request = nullptr;
    return event;
}

bool RrStatusHandler::available() {
    return _request != nullptr;
}

bool RrStatusHandler::consume(Event* event, StateIface* state) {
    _request = event;
    return false;
}