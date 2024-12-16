#include "catagorizer.hpp"

using namespace rrobot;

dlib::logger dlog_c("rr_curator");

void RrCatagorizer::init(StateIface *state, Environment* environment, RrCatagorizerMapper *mapper) {
     dlog_c.set_level(dlib::LALL);
    _state  = state;
    _mapper = mapper;
    _environment = environment;
    EventHandler::init(state->getQueues(), RRP_QUEUES::CATEGORIZER, RRP_QUEUES::USER_INTERFACE);
}

bool RrCatagorizer::consume(Event* event, StateIface* state) {
    RRP_QUEUES queueName = _mapper->mapQueue(event);
    queue<Event*>* queue = state->getQueues()->getQueue(queueName);
    mutex* mtx = state->getQueues()->getLock(queueName);

    const std::lock_guard<std::mutex> lock(*mtx);
    queue->push(event);

    return true;
}

Event* RrCatagorizer::produce(StateIface* state) {return nullptr;}

void RrCatagorizer::setUp() {
    _status = RRP_STATUS::INITILIZING;
    dlog_c << dlib::LINFO << "creating event handlers and initilizing queues";
    vector<EventHandler*> handlers = _mapper->createEventHandlers();

    dlog_c << dlib::LINFO << "creating threads";
    for (EventHandler* handler : handlers) {
         std::thread* t = new std::thread(EventHandler::handleEvent, handler, _state);
         while (!t->joinable()) {
             std::this_thread::sleep_for(std::chrono::milliseconds(_environment->getQueues().getThreadProcessTime()));
         }
         _threads.push_back(t);
         _handlers.push_back(handler);
         t->detach();

         if (handler->status() == RRP_STATUS::TERMINATED || handler->status() == RRP_STATUS::ERROR) {
            dlog_c << dlib::LFATAL << "critical error has occurred while starting handler";
            throw FailedToInitilizeHandler("could not initilize handler");

         }

         while(handler->status() != RRP_STATUS::ACTIVE) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_environment->getQueues().getThreadProcessTime()));
         }
    }

    dlog_c << dlib::LINFO << "starting catagorizer thread";
    _status = RRP_STATUS::ACTIVE;
}

void RrCatagorizer::tearDown() {
    for(EventHandler* handler : _handlers) {
        dlog_c << dlib::LINFO << "waiting for thread to terminate";
        while (handler->status() != RRP_STATUS::TERMINATED) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_environment->getQueues().getThreadProcessTime()));
        }
        dlog_c << dlib::LINFO << "removing handler from thread manager";
        delete(handler);
    }
}

bool RrCatagorizer::produceInt(Event* event) {
    const std::lock_guard<std::mutex> lock(*_outbound_lock);
    _outbound_queue->push(event);
    return true;
}
