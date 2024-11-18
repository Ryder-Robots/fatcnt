#ifndef EVENTPROCESSOR_HPP
#define EVENTPROCESSOR_HPP


using namespace std;

namespace rrobot {
/**
 * @class EventProessor
 * @brief
 * handles inbound, and outbounds events. The following algorithm in followed:
 * 
 * 1. inbound events are recieved are added to the inbound queue. 
 * 2. if inbound queue has more that 0 events, lock queue 
 * 3. for events to min(queue.size, maxEvents)
 * 4.     push events to ai_queue
 * 5. end for
 * 6. raise available message.
 * 7. for outbound queue 
 * 8.    get handler for each event
 * 9.    send event to handler
 * 10. end for
 * 11. check if status is exit 
 * 12. if exit terminate
 * 13. wait for next heartbeat
 */
    class EventProcessor {
    };
}

#endif // EVENTPROCESSOR_HPP