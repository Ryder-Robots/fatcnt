#include <fatcnt/events/eventhandler.hpp>

using namespace rrobot;

dlib::logger dlog_hnd("rr_robot_eventhandler");

void EventHandler::push(Event* event) {
    const std::lock_guard<std::mutex> lock(*_outbound_lock);
    _outbound_queue->push(event);
}

Event* EventHandler::front() {
     const std::lock_guard<std::mutex> lock(*_lock);
     Event* event = _queue->front();
     _queue->pop();
     return event;
}

void EventHandler::handleProduceEvents(EventHandler* handler, StateIface* state) {
    dlog_hnd << dlib::LDEBUG << "producing events for " << handler->name();
    for (int i = 0; i < handler->_limit; i++) {
        if (!handler->available()) {
            break;
        }
        Event* event = handler->produce(state);
        handler->push(event);
    }    
}

void EventHandler::handleConsumeEvents(EventHandler* handler, StateIface* state) {
    dlog_hnd << dlib::LDEBUG << "consuming events for " << handler->name();
    for (int i = 0; i < handler->_limit; i++) {
        if (handler->_queue->empty()) {
            break;
        }
        Event* event = handler->front();
        if (handler->consume(event, state)) {
            dlog_hnd << dlib::LDEBUG << "deleting event for: " << handler->name();
            delete(event);
        }
    }
}

/**
 * @fn handleEvent
 * @brief
 * Uses FIFO queueing to consume and produce events.
 */
void EventHandler::handleEvent(EventHandler* handler, StateIface* state) {
    handler->setStatus(RRP_STATUS::INITILIZING);
    handler->setUp();
    handler->setStatus(RRP_STATUS::ACTIVE);
    RRP_STATUS status = handler->status();

    // isRunning can be considered a kill switch if it is hit, then stop
    while (state->isRunning()) {
        try {
            if (status == RRP_STATUS::ACTIVE) {
                handleProduceEvents(handler, state);
                handleConsumeEvents(handler, state);
            } else if (handler->status() == RRP_STATUS::ERROR) {
                handler->reload();
            }

        } catch (const std::exception& e) {
            state->incremementErrorCount();
            dlog_hnd << dlib::LERROR << "handler: " <<  handler->name() << "reported :" << "error occured while handling event: " << e.what();
            status = RRP_STATUS::ERROR;
            handler->setStatus(status);
            handler->onError(e);
        }
        this_thread::sleep_for(handler->_thread_wait_time);
        status = handler->status();

        if (status != RRP_STATUS::ACTIVE) {
            dlog_hnd << dlib::LWARN << "handler" <<  handler->name() << ": handler status was not active attempting to reload";
            handler->reload();
        }
    }
    handler->setStatus(RRP_STATUS::SHUTTING_DOWN);
    handler->tearDown();
    handler->setStatus(RRP_STATUS::TERMINATED);
}

/**
 * On failure the any exception MUST always set is_exit to true.  This is fatal.
 */
void EventHandler::init(RrQueues* queues, RRP_QUEUES direction, RRP_QUEUES outbound, Environment* environment) {
    dlog_hnd.set_level(environment->getLogging().getLogLevel());

    dlog_hnd << dlib::LINFO << "setting up inbound queue and lock";
    try {
        _queue = queues->getQueue(direction);
        _lock = queues->getLock(direction);
        dlog_hnd << dlib::LINFO << "setting up outbound queue and lock";
        _outbound_queue = queues->getQueue(outbound);
        _outbound_lock = queues->getLock(outbound);
        dlog_hnd << dlib::LINFO << "setting up queue common configuration";
        _thread_wait_time = queues->QUEUE_WAIT_TIME;
        _limit = queues->QUEUE_LIMIT;
    } catch (const QueueDoesNotExit& e) {
        dlog_hnd << dlib::LFATAL << "could not setup inbound and outbound queues";
        throw e;
    }
}
