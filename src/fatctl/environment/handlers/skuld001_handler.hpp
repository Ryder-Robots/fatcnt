#ifndef SKULD001_HANDLER_HPP
#define SKULD001_HANDLER_HPP

#include <string>
#include "fatctl/environment/handlers/handler.hpp"

using namespace std;

namespace rrobot {
    class skuld001_handler : public rr_handler {
    public:
        void init() override;

        int send_action(json action) override;

        json recieve_observation() override;
    };
}

#endif // SKULD001_HANDLER_HPP