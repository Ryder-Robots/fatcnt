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

bool RrCatagorizer::consume(Event* event, StateIface* state) {

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
