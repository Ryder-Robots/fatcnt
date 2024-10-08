/*
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <nlohmann/json.hpp>


using json = nlohmann::json;

/**
 * @brief special shared object that will be used to transmit state to and from UI, Nueral networks, RL, and environment.
 */
namespace rrobot {
    class rr_state_c {
        public:
            /**
             * get current manifest
             */
            virtual json get_manifest() {
                return _manifest;
            }

            virtual void set_manifest(json manifest) {
                _manifest = manifest;
            }

        private:
            json _manifest = json();
    };
}

#endif
