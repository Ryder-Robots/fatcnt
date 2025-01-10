#include <fatcnt/events/eai_handler.hpp>

using namespace rrobot;

dlib::logger dlog_eai("rr_robot_eai");

/*
 * Perform initilization.
 */
void EaiHandler::init(Environment* env, 
    StateIface* state,
    StateManagerIface*  sp, 
    AiGenerateData* agd,
    Serializer<Event*, std::vector<uint8_t>>* s_ctl,  
    Serializer<std::vector<uint8_t>, Event*>* s_out) {

    _env = env;
    _state = state;
    _s_ctl = s_ctl;
    _s_out = s_out;
    _sp = sp;
    _agd = agd;

    dlog_eai.set_level(_env->getLogging().getLogLevel());

    _fc_queue = _env->getAuTrainingData().get_queue_fc();
    _mc_queue = _env->getAuTrainingData().get_queue_mc();

    // User interface instead of micro-controller, or flight controller is chosen as the output queue
    // because where processed actions will vary depending on which drone is getting used.
    EventHandler::init(_state->getQueues(), RRP_QUEUES::AI_ENGINE, RRP_QUEUES::USER_INTERFACE, _env);
}

/*
 * consume event and perform action.
 */
bool EaiHandler::consume(Event* event, StateIface* state) {
    bool rv = false;

    if (_current_mode != _sp->getMode()) {
        dlog_eai << dlib::LWARN << "detected mode change, attempting to change mode";
        setUp();
    }

    switch(_sp->getMode()) {
        case RR_CMODES::CMODE_MANUAL_FLIGHT:
            rv = consume_man_flight(event, state);
            break;

        // Print message to logger.
        case RR_CMODES::CMODE_NOT_SET:
        default:
            // do nothing.
            dlog_eai << dlib::LWARN << "non active mode ignoring";
    }
    return rv;
}


// TODO: complete this. in order to send events _available will need to be set to true.
Event* EaiHandler::produce(StateIface* state) {
    return nullptr;
}

/*
 * indicates that event can be sent back.
 */
bool EaiHandler::available() {
    return _available;
}


bool EaiHandler::consume_man_flight(Event* event, StateIface* state) {

    if (!event->hasPayload()) {
        dlog_eai << dlib::LERROR << "no payload is bound to event - ignoring";
        return false;
    }

    switch(event->getCommand()) {
        case MSPCOMMANDS::MSP_MOTOR:
        {
            std::vector<uint8_t> training = _s_ctl->deserialize(event);            
            Event* fc_event = _s_out->deserialize(training);
            _sp->push_queue(_fc_queue, fc_event);

            std::vector<uint8_t> label = _s_out->serialize(event);
            _agd->write_data(training, label);
        }
        break;

        default:
        dlog_eai << dlib::LERROR << "non supported command was given";
    }

    return true;
}


/*
 * set up write handle
 */
void EaiHandler::setUp() {
    if (_sp->getMode() == RR_CMODES::CMODE_MANUAL_FLIGHT) {
        dlog_eai << dlib::LERROR << "CMODE_MANUAL_FLIGHT detected opening up training file";
        _agd->open_write();
    }
}

void EaiHandler::tearDown() {
    _agd->close_write();
}