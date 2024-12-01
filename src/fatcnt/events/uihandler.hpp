#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "eventhandler.hpp"
#include "ui/authenticator.hpp"
#include <fatcnt/com/external.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

namespace rrobot {
    class UiHandler : public EventHandler {

        public:
            void init(Authenticator *authenticator, External *external, RrQueues* queues, StateIface* state);
            bool consume(Event* event, StateIface* state) override;
            Event* produce(StateIface* state) override;
            bool available() override;

        private:
            Event*        recieve();

            Authenticator* _authenticator;
            External*      _external;
            const char     _delimiter = 0x1E;
            char*          _buffer;
    };
}

#endif