#ifndef CATAGORIZER_HPP
#define CATAGORIZER_HPP

#include <thread>
#include <vector>
#include <fatcnt/state/state.hpp>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/events/catagorizer/catagorizermapper.hpp>
#include <fatcnt/protocols/common/curators/rrp/rrpcurator.hpp>
#include "eventhandler.hpp"

namespace rrobot {

    /**
     * @class RrCatagorizer
     * @brief
     * base classfor catagorizer.
     */
    class RrCatagorizer : public EventHandler {
        public:

            /**
             * @fn init
             * @brief
             * Callled when object is created before thread is created.
             */
            void init(StateIface *state, Environment* environment, RrCatagorizerMapper *mapper, 
                StateManagerIface*  statusProcessor);

            bool consume(Event* event, StateIface* state) override;

            Event* produce(StateIface* state) override;

            bool available() override {return false;}

            string name() override {return "catagorizer";}

            /**
             * @fn setUp
             * @brief
             * creates threads for handlers.
             */
            void setUp() override;

            /**
             * @fn tearDown
             * @brief
             * destroys threads for handlers
             */
            void tearDown() override;

        private:
            RrCatagorizerMapper*  _mapper = nullptr;
            StateIface*           _state  = nullptr;
            std::vector<thread*>  _threads;
            RRP_STATUS            _status = RRP_STATUS::INITILIZING;
            Environment*          _environment;
            StateManagerIface* _statusProcessor = nullptr;

            /**
             * @fn produceInt
             * @brief
             * Produces events that are specific for catagorization, as these events do not rely on
             * an external queue, this is an internal service, they are triggered from the consume 
             * method.
             * 
             */
            bool produceInt(Event* event);
    };
}

#endif // CATAGORIZER_HPP