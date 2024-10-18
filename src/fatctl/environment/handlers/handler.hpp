#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace rrobot {
class rr_handler {
    public:
        /**
         * @fn init
         * @brief perform any hardware initlization.
         */
        virtual void init() = 0;

        /**
         * @fn send_action
         * @brief sends an action to hardware
         */
        virtual int send_action(json action) = 0;

        /**
         * @fn recieve_observation
         * @brief recieves an observation from hardware.
         */
        virtual json recieve_observation() = 0;
};
}

#endif