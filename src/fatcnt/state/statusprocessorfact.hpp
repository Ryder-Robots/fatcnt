#ifndef STATUSPROCESSORFACT_HPP
#define STATUSPROCESSORFACT_HPP

#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/stateiface.hpp>
#include "statusprocessor.hpp"

namespace rrobot {
    /**
     * @class StatusProcessorFactory
     * @brief
     * Factory class to create StatusProcessor.
     */
    class StatusProcessorFactory {
        public:
        /**
         * @fn createStatusProcessor
         * @brief
         * based on environment, and state create a status processor.
         */
        static StatusProcessorIface* createStatusProcessor(Environment env, StateIface* state);
    };
}

#endif // STATUSPROCESSORFACT_HPP