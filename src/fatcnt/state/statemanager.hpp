#ifndef STATUSPROCESOR_HPP
#define STATUSPROCESOR_HPP

#include <stdint.h>

#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/state/statemanageriface.hpp>
#include <mutex>

using namespace std;

namespace rrobot {
class StateManager : public StateManagerIface {
   public:
    StateManager(StateIface* state, Environment* environment) : _state(state), _environment(environment) {}

    RR_CMODES getMode() override;

    RRP_STATUS getStatus() override;

    void addHandler(EventHandler* handler) override { _eventHandlers.push_back(handler); }

    vector<EventHandler*> getHandlers() { return _eventHandlers; }

    void setMode(RR_CMODES mode) override;

    StateIface* getState() {return _state;}

    Environment* getEnv() override {return _environment;}

    int32_t getFlags() override;

   private:
    vector<EventHandler*> _eventHandlers = {};
    StateIface* _state;
    mutex _lock;
    Environment* _environment;

    // Environment*       _environment;
    const int32_t DRONE_ARMED = 0 | INITILIZING | ERROR | RELOADING | SHUTTING_DOWN | TERMINATED;
};
}  // namespace rrobot

#endif  // STATUSPROCESOR_HPP