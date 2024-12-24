#ifndef CATAGORIZERMAPPERBASE_HPP
#define CATAGORIZERMAPPERBASE_HPP

#include <mutex>
#include <fatcnt/protocols/common/curators/rrp/msp_mode.hpp>
#include "catgorizermapper.hpp"

namespace rrobot {

    /**
     * @class CatagorizerMapperBase
     * @brief
     * base class for catagorizermapper, provides mode mapping.
     */
    class CatagorizerMapperBase : RrCatagorizerMapper {
        public:
            void init(Environment* environment, StateIface* state, StatusProcessorIface* statusProcessor) override {
                _environment = environment;
                _state = state;
                _statusProcessor = statusProcessor;
            }

        protected:
            void setMode(RR_CMODES mode) {
               _statusProcessor->setMode(mode);
            }

            RR_CMODES getMode() {
                return _statusProcessor->getMode();
            }

            Environment* _environment = nullptr;
            StateIface*  _state = nullptr;
            StatusProcessorIface* _statusProcessor;

        protected:
            std::mutex   _lock;
    };
}

#endif // CATAGORIZERMAPPERBASE_HPP