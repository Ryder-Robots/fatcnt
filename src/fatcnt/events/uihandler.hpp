#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/com/external.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/state/rrpqueues.hpp>

#include "eventhandler.hpp"

using json = nlohmann::json;

namespace rrobot {

    /**
     * @class UiHandler
     * 
     * @brief
     * Handles network requests, this is inteneded for WIFI, WAN,  but could possibly could be used for 
     * radio controller events.
     */
    class UiHandler : public EventHandler {

        public:
            /**
             * @fn init
             * @brief
             * initlize handler.
             */
            void init(External *external, Environment* environment, StateIface* state, Serializer<json, Event*>* serializer);

            /**
             * @fn consume
             * @brief
             * serialize events sent from other processors and send them to connection.
             */
            bool consume(Event* event, StateIface* state) override;

            /**
             * @fn produce
             * @brief
             * recieve events from connection, deserialize and return the deserialized event.
             */
            Event* produce(StateIface* state) override;

            /**
             * process is available for operation.
             */
            bool available() override;

            void setUp() override;

            void reload() override;

            void tearDown() override;

            string name() override {return "uihandler";}

        private:
            External*         _external;
            const char        _delimiter = 0x1E;
            char*             _buffer;
            Serializer<json, Event*>* _serializer;
            bool              _available = true;
            Environment      *_environment = nullptr;
            StateIface       *_state = nullptr;
    };
}

#endif