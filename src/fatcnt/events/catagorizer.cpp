#include "catagorizer.hpp"

using namespace rrobot;

dlib::logger dlog_c("rr_curator");

void RrCatagorizer::init(RrQueues* queues, StateIface *state, Environment* environment, RrCatagorizerMapper *mapper) {
     dlog_c.set_level(dlib::LALL);
    _state  = state;
    _mapper = mapper;
    _environment = environment;
    EventHandler::init(queues, RRP_QUEUES::CATEGORIZER, RRP_QUEUES::USER_INTERFACE);
}


/**
 * iterate through each handler,  and set the corresponding bit value. If values are anything but
 * 2, then not all components have initialized. For each iteration of flags is set to '0' therefore
 * it should be seen as the current state.
 */
int32_t RrCatagorizer::getFlags() {
    int32_t flags = 0;
    for (auto handler : _handlers) {
        flags = flags | handler->status();
    }
    return flags;
 }

bool RrCatagorizer::produceRequest(MSPCOMMANDS request) {
    
    void* payload = nullptr;
    Environment env = *_environment;
    StateIface* state = _state;

    switch(request) {
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

    if (payload == nullptr) {
        dlog_c << dlib::LERROR << "an unknown request was sent, ignoring";
        return false;
    }
    Event* event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_OUT, payload);
    produceInt(event);
    return true;
}

bool RrCatagorizer::consume(Event* event, StateIface* state) {

    // inbound requests go here. (this should be a separate method.)
    if (event->getDirection() == MSPDIRECTION::EXTERNAL_IN) {
        // check for mode changes, 
        if (event->getCommand() == MSP_MODE) {
            if (!event->hasPayload()) {
                dlog_c << dlib::LERROR << "payload is a required attribute";
                throw MissingRequiredAttributeException("payload is a required attribute");
            }
            msp_mode mode = event->getPayload<msp_mode>();
            if (!_mapper->initializeMode(_environment, _state, mode.get_mode())) {
                dlog_c << dlib::LWARN << "can not switch to requested mode";
                {
                    msp_error* mspError = new msp_error();
                    mspError->set_message("request mode is unavaiable");
                    Event* event = new Event(MSPCOMMANDS::MSP_ERROR, MSPDIRECTION::ERROR, mspError);
                    produceInt(event);
                }
            }
        } else {
            produceRequest(event->getCommand());
        }
        return true;
    }

    RRP_QUEUES queueName = _mapper->mapQueue(_environment, state, event);
    queue<Event*>* queue = _queues->getQueue(queueName);
    mutex* mtx = _queues->getLock(queueName);

    const std::lock_guard<std::mutex> lock(*mtx);
    queue->push(event);

    return true;
}

Event* RrCatagorizer::produce(StateIface* state) {return nullptr;}

void RrCatagorizer::setUp() {
    _status = RRP_STATUS::INITILIZING;
    dlog_c << dlib::LINFO << "creating event handlers and initilizing queues";
    vector<EventHandler*> handlers = _mapper->createEventHandlers(_environment, _state);

    dlog_c << dlib::LINFO << "creating threads";
    for (EventHandler* handler : handlers) {
         std::thread* t = new std::thread(EventHandler::handleEvent, handler, _state);
         while (!t->joinable()) {
             std::this_thread::sleep_for(std::chrono::milliseconds(10));
         }
         _threads.push_back(t);
         _handlers.push_back(handler);
         t->detach();
    }

    dlog_c << dlib::LINFO << "starting catagorizer thread";
    _status = RRP_STATUS::ACTIVE;
}

void RrCatagorizer::tearDown() {
    for(EventHandler* handler : _handlers) {
        dlog_c << dlib::LINFO << "waiting for thread to terminate";
        while (handler->status() != RRP_STATUS::TERMINATED) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

bool RrCatagorizer::produceInt(Event* event) {
    const std::lock_guard<std::mutex> lock(*_outbound_lock);
    _outbound_queue->push(event);
    return true;
}


void RrCatagorizer::onError(const std::exception& e) {
    dlog_c << dlib::LERROR << "and unhandled exception has occurred: " + string(e.what());
    StateIface* state = _state;

    state->incremementErrorCount();
}