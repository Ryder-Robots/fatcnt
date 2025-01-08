#ifndef AI_LISTENER_HPP
#define AI_LISTENER_HPP

#include "eventhandler.hpp"

namespace rrobot {

    /**
     * @class AiListener
     * @brief
     * controller for inbound AI events, depending current drone mode, determines what to do with
     * inbound request.
     */
    class AiListener : public EventHandler {

    };
}

#endif // AI_LISTENER_HPP