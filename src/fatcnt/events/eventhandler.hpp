#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <dlib/logger.h>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/events/Event.hpp>

using namespace std;

namespace rrobot {

    /**
     * @class EventHandler
     * 
     * @brief
     * Event handlers, handle a specific type of event.
     */
    class EventHandler {
        protected:

        /**
         * @fn init
         * @param environment environment class
         * @brief
         * This method is called when program is reloaded, or initialization time by the queue manger
         * 
         */
        void init(Queues qconfig, RrQueues queues, MSPDIRECTION inbound, MSPDIRECTION outbound);
        static void handleEvent(EventHandler *handler);
        
        virtual void consume(Event event) = 0;
        virtual Event produce() = 0;
        virtual bool available() = 0;

        State        *_state;
        queue<Event>* _queue;
        mutex*        _lock;
        
        int           _limit;
        chrono::milliseconds _thread_wait_time;
        queue<Event>* _outbound_queue;
        mutex*        _outbound_lock;

    };
}

#endif // EVENTHANDLER_HPP