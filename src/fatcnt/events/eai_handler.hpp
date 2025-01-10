#ifndef EAI_HANDLER_HPP
#define EAI_HANDLER_HPP

#include <cstdlib>
#include <vector>
#include <dlib/dnn.h>
#include <fatcnt/state/statemanageriface.hpp>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/events/ai/ai_training_data.hpp>

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
         * @param state processor
         * @param inbound serilizer
         * @param outbound serializer
         */
        void init(Environment* env, 
            StateIface* state,
            StateManagerIface*  sp,
            AiGenerateData* agd,
            Serializer<Event*, std::vector<uint8_t>>* s_ctl,  
            Serializer<std::vector<uint8_t>, Event*>* s_out);

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

        void setUp() override;

        void tearDown() override;

        private:
        Environment* _env;
        StateIface* _state;
        Serializer<Event*, std::vector<uint8_t>>* _s_ctl;
        Serializer<std::vector<uint8_t>, Event*>* _s_out;
        StateManagerIface* _sp;
        AiGenerateData* _agd;
        RRP_QUEUES _fc_queue;
        RRP_QUEUES _mc_queue;
        bool _available = false;
        RR_CMODES _current_mode = RR_CMODES::CMODE_NOT_SET;

        bool consume_man_flight(Event* event, StateIface* state);
    };
}

#endif // EAI_HANDLER_HPP