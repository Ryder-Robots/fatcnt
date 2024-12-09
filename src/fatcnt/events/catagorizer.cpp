#include "catagorizer.hpp"

using namespace rrobot;

dlib::logger dlog_c("rr_curator");

void RrCatagorizer::init(RrQueues* queues, StateIface *state, Environment environment, RrCatagorizerMapper *mapper) {
    
     dlog_c.set_level(dlib::LALL);
    _state  = state;
    _mapper = mapper;

    dlog_c << dlib::LINFO << "creating event handlers and initilizing queues";
    EventHandler::init(queues, MSPDIRECTION::CATEGORIZER, MSPDIRECTION::USER_INTERFACE);
    vector<EventHandler*> handlers = mapper->createEventHandlers(environment, *state);

    dlog_c << dlib::LINFO << "creating threads";
    for (EventHandler* handler : handlers) {
        std::thread* t = new std::thread(EventHandler::handleEvent, handler, state);
        while (!t->joinable()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        _threads.push_back(t);
    }

    dlog_c << dlib::LINFO << "starting catagorizer thread";
    thread* t = new thread(RrCatagorizer::handleEvent, this, state);
    _cthread = t;
}

bool RrCatagorizer::consume(Event* event, StateIface* state) {
    if (event->getCommand() == MSPCOMMANDS::MSP_AUTHKEY) {      
    }

    return true;
}

Event* RrCatagorizer::produce(StateIface* state) {return nullptr;}

void RrCatagorizer::setUp() {
    _status = RRP_STATUS::ACTIVE;
}

void RrCatagorizer::tearDown() {

    for (thread* t : _threads) {
        while (!t->joinable()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        t->join();
    }
    _status = RRP_STATUS::SHUTTING_DOWN; 
}

bool RrCatagorizer::produceInt(Event* event) {
    const std::lock_guard<std::mutex> lock(*_outbound_lock);
    _outbound_queue->push(event);
}