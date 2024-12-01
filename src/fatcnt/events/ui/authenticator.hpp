#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <fatcnt/events/Event.hpp>

namespace rrobot {
    class Authenticator {
        public:
        /**
         * @fn authenicate
         * @brief
         * if authenticated returns true, otherwise returns false.
         */
        virtual bool authenticate(Event* event) = 0;
    };
}

#endif // AUTHENTICATOR_HPP