#ifndef AI_VIRTUAL_PREDICTOR_HPP
#define AI_VIRTUAL_PREDICTOR_HPP

#include <fatcnt/events/ai/ai_predictor.hpp>
#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

namespace rrobot {
    class AiVirtualPredictor : public AiPredictor {

        public:
        /**
         * @fn init
         * @brief
         * 
         * Virtual interface ignores intialization state
         */
        void init(StateIface *state, Environment *environment, StateManagerIface* statusProcessor) override {}

        void addFeature(Event* event) override;

        Event* predict() override;

        private:
        Event* _event; 
        Msp104Serializer* _convertor = new Msp104Serializer();
    };
}

#endif