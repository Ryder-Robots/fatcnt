#ifndef MSPDIRECTION_HPP
#define MSPDIRECTION_HPP

namespace rrobot {

/**
 * @brief 
 * describes intended destination queue.  This can be overridden by EventProcessor,  for example a message MUST not 
 * be sent to AI_ENGINE, or FLIGHT_CONTROLLER unless connection has been authenticated.
 */
enum class MSPDIRECTION {
    USER_INTERFACE,
    AI_ENGINE,
    AUHTH,
    MICROCONTROLLER,
    FLIGHT_CONTROLLER,
    INVALID,
};
}

#endif  // MSPDIRECTION_HPP