#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <dlib/logger.h>

#include <chrono>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/protocols/rrp/rrpstatus.hpp>
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

    /**
     * @fn handleEvent
     * @brief
     * Called within the thread to allow execution of the handler.
     * 
     * @param handler (this object)
     * @param state  reference to state object
     */
    static void handleEvent(EventHandler* handler, StateIface *state);

    /**
     * @fn consume
     * @brief
     * this method must be implemented when another handler directs an event to this handler.
     * @param event event that is to be consumed
     * @param state reference to current state file.
     * @return true on success.
     */
    virtual bool consume(Event* event, StateIface* state) = 0;

    /**
     * @fn produce
     * @brief 
     when available method returns true, then call this method that will produce events for
     other handlers.
     */
    virtual Event* produce(StateIface* state) = 0;
    
    /**
     * @fn available
     * @brief
     * when events can be consumed this method must return true.
     */
    virtual bool available() = 0;


    /**
     * Current status of the handler.
     */
    virtual RRP_STATUS status() = 0;

    /**
     * @fn setUp
     * @brief
     * if defined by the handler performs any start up routine necessary
     */
    virtual void setUp() {}

    /**
     * @fn shutDown
     * @brief
     * called when isRunning() == false and when defined by handler indicates that the drone should be taken offline.
     */
    virtual void tearDown() {}

    /**
     * @fn reload
     * @brief
     * called to reload system after an error has occured.
     */
    virtual void reload() {}

   private:

    static void handleProduceEvents(EventHandler* handler, StateIface* state);
    static void handleConsumeEvents(EventHandler* handler, StateIface* state);

    queue<Event*>* _queue;
    mutex* _lock;
    int _limit;
    chrono::milliseconds _thread_wait_time;
    queue<Event*>* _outbound_queue;
    mutex* _outbound_lock;
};
}  // namespace rrobot

#endif  // EVENTHANDLER_HPP