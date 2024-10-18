/*
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <queue>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "fatctl/ui/websocket_status_code.hpp"
#include "fatctl/environment/handlers/handler.hpp"

using json = nlohmann::json;

/**
 * @brief special shared object that will be used to transmit state to and from UI, Nueral networks, RL, and
 * environment.
 */
namespace rrobot {
class rr_state_c {
   public:
    /**
     * get current manifest
     */
    json get_manifest();

    /**
     * set manifest file
     */
    void set_manifest(json manifest);

    /**
     * @fn put_send_event
     * @param JSON event to send.
     */
    /*
     * at this stage plave event on a  buffer so it can be used later.
     */
    void put_event(json event);

    /**
     * @fn get_event
     * @brief return last event from event queue
     * @return last event as JSON
     */
    json get_event();

    /**
     * @fn init
     * @brief performs any initilization tasks, must be called before using class.
     */
    void init();

    void set_handler(rr_handler *handler) {
        _handler = handler;
    }

    rr_handler *get_handler() {
        return _handler;
    }

    bool events_empty() {
        return _event_queue.empty();
    }

   private:
    json _manifest = json();
    std::queue<std::string> _event_queue;
    pthread_mutex_t _event_lock;
    rr_handler *_handler;
};
}  // namespace rrobot

#endif
