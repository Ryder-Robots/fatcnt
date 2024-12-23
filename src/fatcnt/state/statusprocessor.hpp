#ifndef STATUSPROCESOR_HPP
#define STATUSPROCESOR_HPP

#include <fatcnt/state/statusprocessoriface.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/events/eventhandler.hpp>
#include <mutex>

using namespace std;

namespace rrobot {
    class StatusProcessor : StatusProcessorIface {
        public:
            StatusProcessor(StateIface* state): _state(state) {}

            RR_CMODES getMode() override;
            
            RRP_STATUS getStatus() override;

            void addHandler(EventHandler* handler) override {
                _eventHandlers.push_back(handler);
            }

            vector<EventHandler*> getHandlers() {
                return _eventHandlers;
            }

            void setMode(RR_CMODES mode) override;

        private:
        vector<EventHandler*> _eventHandlers = {};
        StateIface*           _state;
        mutex                 _lock;
        
        
        const int32_t DRONE_ARMED = 0 | INITILIZING | ERROR | RELOADING | SHUTTING_DOWN | TERMINATED; 
    };
}

#endif // STATUSPROCESOR_HPP