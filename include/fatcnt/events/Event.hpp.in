#ifndef EVENT_HPP
#define EVENT_HPP

#include <stdlib.h>
#include <fatcnt/protcols/common/mspdirection.hpp>
#include <fatcnt/protcols/common/mspcommands.hpp>

namespace rrobot {
/**
 *  @brief events are added to queues.
 */
class Event {
   public:
    Event(MSPCOMMANDS command, MSPDIRECTION direction, void* payload)
        : _command(command), 
          _direction(_direction),
          _payload(payload) {}

    /**
     * @fn getPayload
     * @brief 
     * Returns the payload
     * @return payload
     */
    template <typename T> T getPayload() {
        return *(reinterpret_cast<T*>(_payload));
    }

   private:
    const MSPCOMMANDS  _command;   // command
    const MSPDIRECTION _direction; // direction of command
    const void*        _payload;   // payload
};
}  // namespace rrobot

#endif  // EVENT_HPP