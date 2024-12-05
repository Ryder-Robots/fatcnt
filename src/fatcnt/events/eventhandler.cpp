#include "eventhandler.hpp"

using namespace rrobot;

dlib::logger dlog_hnd("rr_robot_eventhandler");

/**
 * @fn handleEvent
 * @brief
 * Uses FIFO queueing to consume and produce events.
 */
void EventHandler::handleEvent(EventHandler* handler, StateIface* state) {
    // isRunning can be considered a kill switch if it is hit, then stop
    while (state->isRunning()) {
        try {
            handler->_outbound_lock->lock();
            for (int i = 0; i < handler->_limit; i++) {
                if (!handler->available()) {
                    break;
                }
                handler->_outbound_queue->push(handler->produce(state));
            }
            handler->_outbound_lock->unlock();

            handler->_lock->lock();
            for (int i = 0; i < handler->_limit; i++) {
                if (handler->_queue->empty()) {
                    break;
                }

                Event* event = handler->_queue->front();
                handler->consume(event, state);
                handler->_queue->pop();
            }
            handler->_lock->unlock();

        } catch (const std::exception& e) {
            dlog_hnd << dlib::LERROR << "error occured while handling event " << e.what();
        }
        this_thread::sleep_for(handler->_thread_wait_time);
    }
}

/**
 * On failure the any exception MUST always set is_exit to true.  This is fatal.
 */
void EventHandler::init(RrQueues* queues, MSPDIRECTION direction, MSPDIRECTION outbound) {
    dlog_hnd.set_level(dlib::LALL);

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
