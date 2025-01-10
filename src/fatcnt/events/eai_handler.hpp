#ifndef EAI_HANDLER_HPP
#define EAI_HANDLER_HPP

#include <cstdlib>
#include <vector>
#include <dlib/dnn.h>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/events/eventhandler.hpp>

using namespace dlib;
using namespace std;

namespace rrobot {
    /**
     * @class EaiHandler
     * @brief
     * Interface to AI components of the drone.
     */
    class EaiHandler : public EventHandler {
        public:

        /**
         * @fn init
         * @brief
         * inilizes EAI handler, including underlaying handler layer.
         * @param env global environment.
         * @param state current state
         * @param inbound serilizer
         * @param outbound serializer
         */
        void init(Environment* env, StateIface* state, Serializer<Event*, std::vector<uint8_t>>,  Serializer<std::vector<uint8_t>, Event*> out);

        /**
         * @fn name
         * @brief
         * returns eai_handler for logging purposes.
         */
        string name() override {return "eai_handler";}
        
        /**
         * @fn consume
         * @brief
         * after recieving an event checks flags (mode) to determine how to deal with the 
         * request.
         * 
         * For modes
         * - CMODE_NOT_SET do nothing with the request
         * - CMODE_MANUAL_FLIGHT onbound request, and generated reponse to file.
         */
        bool consume(Event* event, StateIface* state) override;

        /**
         * @fn produce
         * @brief
         * return status event such as the current status of training data.
         */
        Event* produce(StateIface* state) override;

        /**
         * @brief
         * available
         * @brief
         * handler is ready to recieve next request.
         */
        bool available() override;
    };
}

#endif // EAI_HANDLER_HPP