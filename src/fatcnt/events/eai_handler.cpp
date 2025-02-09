#include <fatcnt/events/eai_handler.hpp>

using namespace rrobot;

dlib::logger dlog_eai("rr_robot_eai");

/*
 * Perform initilization.
 */
void EaiHandler::init(
    StateIface* state,
    Environment* env,
    StateManagerIface*  sp,
    AiPredictor* s_ctl) {

    _env = env;
    _state = state;
    _s_ctl = s_ctl;
    _sp = sp;

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

        // Put prediction modes at top, they can fall through to predict() method.
        case RR_CMODES::CMODE_MANUAL_FLIGHT:
            rv = predict(event, state);
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

/*
 * Recieve flight controller event, serialize the event based upon requirements of DNN, 
 * get preditction form DNN and send the event to a closed loop.
 */
bool EaiHandler::predict(Event* event, StateIface* state) {

    if (!event->hasPayload()) {
        dlog_eai << dlib::LERROR << "no payload is bound to event - ignoring";
        return false;
    }
    dlog_eai << dlib::LDEBUG << "AI command";
    _s_ctl->addFeature(event);
    Event* fc_event = _s_ctl->predict();
    _sp->push_queue(_fc_queue, fc_event);


    return true;
}

/*
 * set up write handle
 */
void EaiHandler::setUp() {
    if (_sp->getMode() == RR_CMODES::CMODE_MANUAL_FLIGHT) {
        dlog_eai << dlib::LINFO << "CMODE_MANUAL_FLIGHT detected opening up training file";
        _current_mode = RR_CMODES::CMODE_MANUAL_FLIGHT;
    } else if (_sp->getMode() == RR_CMODES::CMODE_NOT_SET) {
        dlog_eai << dlib::LINFO << "CMODE_NOT_SET detected changing mode";
         _current_mode = RR_CMODES::CMODE_NOT_SET;
    } else {
        dlog_eai << dlib::LERROR << "unsupported mode was sent, no idea what to do with this!";
    }
}

void EaiHandler::tearDown() {
    
}