#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <dlib/logger.h>

#include <chrono>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <mutex>
#include <queue>

using namespace std;

namespace rrobot {

/**
 * @class EventHandler
 *
 * @brief
 * Event handlers, handle a specific type of event.
 */
class EventHandler {
   public:
    /**
     * @fn init
     * @param environment environment class
     * @brief
     * This method is called when program is reloaded, or initialization time by the queue manger
     *
     */
    void init(RrQueues* queues, MSPDIRECTION inbound, MSPDIRECTION outbound);
    static void handleEvent(EventHandler* handler, StateIface *state);
    virtual void consume(Event event, StateIface* state) = 0;
    virtual Event produce(StateIface* state) = 0;
    virtual bool available() = 0;

   private:
    queue<Event>* _queue;
    mutex* _lock;

    int _limit;
    chrono::milliseconds _thread_wait_time;
    queue<Event>* _outbound_queue;
    mutex* _outbound_lock;
};
}  // namespace rrobot

#endif  // EVENTHANDLER_HPP