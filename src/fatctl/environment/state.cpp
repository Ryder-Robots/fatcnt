#include "fatctl/environment/state.hpp"

using namespace rrobot;

json rr_state_c::get_manifest() { return _manifest; }

void rr_state_c::set_manifest(json manifest) { _manifest = manifest; }


void rr_state_c::put_event(json event) { 
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

json rr_state_c::get_event() {
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

void rr_state_c::init() {
    if (pthread_mutex_init(&_event_lock, NULL) != 0) {
        throw RR_MUTEX_ERR;
    }
}