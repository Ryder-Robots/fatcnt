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
            void init(Environment* environment, StateIface* state) override {
                _environment = environment;
                _state = state;
            }

        protected:
            void setMode(RR_CMMODES mode) {
                const std::lock_guard<std::mutex> lock(_lock);
                _mode = mode;
            }

            RR_CMMODES getMode() {
                return _mode;
            }

            Environment* _environment = nullptr;
            StateIface*  _state = nullptr;
            RR_CMMODES   _mode = CMODE_NOT_SET;

        protected:
            std::mutex   _lock;
    };
}

#endif // CATAGORIZERMAPPERBASE_HPP