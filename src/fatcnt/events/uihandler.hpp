#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/com/external.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

#include "eventhandler.hpp"
#include "ui/authenticator.hpp"

using json = nlohmann::json;

namespace rrobot {
    class UiHandler : public EventHandler {

        public:
            void init(Authenticator *authenticator, External *external, StateIface* state, Serializer<json> serializer);
            bool consume(Event* event, StateIface* state) override;
            Event* produce(StateIface* state) override;
            bool available() override;

        private:
            Event*          recieve();

            Authenticator*   _authenticator;
            External*        _external;
            const char       _delimiter = 0x1E;
            char*            _buffer;
            Serializer<json> _serializer;
    };
}

#endif