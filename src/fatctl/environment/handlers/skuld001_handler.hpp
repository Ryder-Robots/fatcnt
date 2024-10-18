#ifndef SKULD001_HANDLER_HPP
#define SKULD001_HANDLER_HPP

#include "fatctl/environment/handlers/skuld001_handler.hpp"

namespace rrobot {
    class skuld001_handler {
    public:
        void init();

        static void *handle_operations(void *state_in);
    };
}

#endif // SKULD001_HANDLER_HPP