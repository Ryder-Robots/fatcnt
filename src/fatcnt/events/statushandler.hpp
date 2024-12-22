#ifndef STATUSHANDLER_HPP
#define STATUSHANDLER_HPP

#include <stdint.h>
#include <vector>
#include <fatcnt/protocols/common/curators/rrp/mspevent.hpp>
#include "eventhandler.hpp"

using namespace std;

namespace rrobot {
    /**
     * @class RrStatusHandler
     * @brief
     * handles status requests and returns directly to the user interface.
     */
    class RrStatusHandler : public EventHandler {
        public:
        bool consume(Event* event, StateIface* state) override;

        Event* produce(StateIface* state) override;

        /**
         * @fn available
         * @brief
         * if request is available return true.
         */
        bool available() override;

        bool isArmed();

        /**
         * @fn init
         * @brief
         * base initlization.
         */
        void init(StateIface *state, Environment* environment, vector<EventHandler *> handlers);

        string name() override {return "statushandler";}

        int32_t getFlags();

        private:
        Event* _request = nullptr;
        Environment* _environment = nullptr;
        vector<EventHandler*> _handlers;
        StateIface* _state;

        const int32_t DRONE_ARMED = 0 | INITILIZING | ERROR | RELOADING | SHUTTING_DOWN | TERMINATED; 
    };

}
#endif // STATUSHANDLER_HPP