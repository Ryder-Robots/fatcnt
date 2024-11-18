#ifndef EVENT_HPP
#define EVENT_HPP

#include <stdlib.h>
#include <fatcnt/protcols/common/MspDirection.hpp>
#include <fatcnt/protcols/common/MspCommands.hpp>

namespace rrobot {
/**
 *  @brief events are added to queues.
 */
class Event {
   public:
    Event(MspCommands command, MSDIRECTION direction, void* payload)
        : _command(command), 
          _direction(_direction),
          _payload(payload) {}

    ~Event() { free(_payload); }


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
    const MspCommands _command;   // command
    const MSPDIRCTION _direction; // direction of command
    const void*       _payload;   // payload
};
}  // namespace rrobot

#endif  // EVENT_HPP