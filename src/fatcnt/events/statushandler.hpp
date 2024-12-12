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
        bool consume(Event* event, StateIface* state) override;

        Event* produce(StateIface* state) override;

        /**
         * @fn available
         * @brief
         * if request is available return true.
         */
        bool available() override;

        /**
         * @fn init
         * @brief
         * base initlization.
         */
        void init(RrQueues* queues, Environment* environment, vector<EventHandler *> handlers);

        private:
        Event* _request = nullptr;
        Environment* _environment = nullptr;
        vector<EventHandler*> _handlers;

        int32_t getFlags();
    };
}
#endif // STATUSHANDLER_HPP