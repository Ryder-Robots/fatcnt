#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <dlib/logger.h>

#include <chrono>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/state/statefactory.hpp>
#include <fatcnt/state/rrpqueues.hpp>
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
    void init(RrQueues* queues, RRP_QUEUES inbound, RRP_QUEUES outbound, Environment* environment);

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
     * @fn name
     * @brief
     * The name of the handler used for debugging purposes.
     */
    virtual string name() = 0;

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
    RRP_STATUS status() {
        return _status;
    }

    void setStatus(RRP_STATUS status) {
        _status = status;
    }

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
     * called to reload system after an error has occured.  If this method is implemented then the handler MUST
     * reset the status is the error has been handled.
     */
    virtual void reload() {}

    /**
     * @fn onError
     * @brief 
     * when in implmented called when an error has occured within handler.
     * 
     * It is up to the handler to set the status to active if the exception has been handled.
     * 
     * @param exception that has occured.
     */
    virtual void onError(const std::exception& e) {}

   protected:
    queue<Event*>* _queue;
    mutex* _lock;
    queue<Event*>* _outbound_queue;
    mutex* _outbound_lock;

   private:

    static void handleProduceEvents(EventHandler* handler, StateIface* state);
    static void handleConsumeEvents(EventHandler* handler, StateIface* state);

    int _limit;
    chrono::milliseconds _thread_wait_time;

    RRP_STATUS        _status = RRP_STATUS::INITILIZING;
};
}  // namespace rrobot

#endif  // EVENTHANDLER_HPP