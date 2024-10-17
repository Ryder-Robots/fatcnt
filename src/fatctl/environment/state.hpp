/*
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <queue>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "fatctl/ui/websocket_status_code.hpp"

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
    virtual json get_manifest() { return _manifest; }

    virtual void set_manifest(json manifest) { _manifest = manifest; }

    /**
     * @fn put_send_event
     * @param JSON event to send.
     */
    /*
     * at this stage plave event on a  buffer so it can be used later.
     */
    void put_event(json event) { 
        bool ex_thrown = false;
        try {
            std::string msg = event.dump();
            pthread_mutex_lock(&_event_lock);
            _event_queue.push(msg);
        } catch (...) {
            ex_thrown = true;
        }
        
        // Release lock before throwing exception.
        pthread_mutex_unlock(&_event_lock);
        if (ex_thrown) {
            throw std::runtime_error("unable to add event to queue");
        }
    }

    /**
     * @fn get_event
     * @brief return last event from event queue
     * @return last event as JSON
     */
    json get_event() {
        bool ex_thrown = false;
        json res = json();
        try {
            pthread_mutex_lock(&_event_lock);
            if (!_event_queue.empty()) {
                std::string front = _event_queue.front();
                _event_queue.pop();
                res = json::parse(front);
            }
        } catch(...) {
            ex_thrown = true;
        }

        // Release lock before throwing exception.
        pthread_mutex_unlock(&_event_lock);
        if (ex_thrown) {
            throw std::runtime_error("unable to retrieve event");
        }
        return res; 
    }

    /**
     * @fn init
     * @brief performs any initilization tasks, must be called before using class.
     */
    void init() {
        if (pthread_mutex_init(&_event_lock, NULL) != 0) {
            throw RR_MUTEX_ERR;
        }
    }

   private:
    json _manifest = json();
    std::queue<std::string> _event_queue;
    pthread_mutex_t _event_lock; 
};
}  // namespace rrobot

#endif
