#include "eventhandler.hpp"

using namespace rrobot;

dlib::logger dlog_hnd("rr_robot_eventhandler");

/**
 * @fn handleEvent
 * @brief
 * Uses FIFO queueing to consume and produce events.
 */
void EventHandler::handleEvent(EventHandler *handler) {

    // isRunning can be considered a kill switch if it is hit, then stop
    while (handler->_state->isRunning()) {
        if (!handler->_state->isAuthenticated()) {
            dlog_hnd << dlib::LWARN << "attempt to read or write to queue while not authenticated";
            return;
        }

        try {
            handler->_outbound_lock->lock();
            for (int i = 0; i < handler->_limit; i++) {
                if (!handler->available()) {
                    break;
                }
                handler->_outbound_queue->push(handler->produce());
            }
            handler->_outbound_lock->unlock();

            handler->_lock->lock();
            for (int i = 0; i < handler->_limit; i++) {
                if (handler->_queue->empty()) {
                    break;
                }

                Event event = handler->_queue->front();
                handler->consume(event);
                handler->_queue->pop();
            }
            handler->_lock->unlock();
            
        } catch (const std::exception& e) {
            dlog_hnd << dlib::LERROR << "error occured while handling event " << e.what();
        }
        this_thread::sleep_for(handler->_thread_wait_time);
    }
}

void EventHandler::init(Queues qconfig, RrQueues queues, MSPDIRECTION direction, MSPDIRECTION outbound) {
    dlog_hnd.set_level(dlib::LALL);

    dlog_hnd << dlib::LINFO << "setting up inbound queue and lock";
    _queue = _state->getQueues()->getQueue(direction);
    _lock = _state->getQueues()->getLock(direction);

    _outbound_queue = _state->getQueues()->getQueue(outbound);
    _outbound_lock = _state->getQueues()->getLock(outbound);

    dlog_hnd << dlib::LINFO << "setting up outbound queue and lock";
}
