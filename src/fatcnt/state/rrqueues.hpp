#ifndef RRQUEUE_HPP
#define RRQUEUE_HPP

#include <mutex>
#include <string>
#include <queue>
#include <chrono>
#include <unordered_map>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

using namespace std;

namespace rrobot {
    /**
     * @class RrQueues
     * @brief
     * Contains queues, and pthread conditions
     */
    class RrQueues {
        public:
            RrQueues(int queue_limit, chrono::milliseconds queue_wait_time, chrono::milliseconds queue_process_time):
                QUEUE_LIMIT(queue_limit),
                QUEUE_WAIT_TIME(queue_wait_time),
                QUEUE_PROCESS_TIME(queue_process_time) {}

            // maximum events that can be processeed before sleeping, 
            // or written to a queue
            const int QUEUE_LIMIT; 
            chrono::milliseconds QUEUE_WAIT_TIME;    // time handler must wait after processing queue
            chrono::milliseconds QUEUE_PROCESS_TIME; // how long a queue can be processed for
            queue<Event> _q_events;    // events that are yet to be catorgrized.
            mutex _l_events; // locak for inbound events

            /**
             * @fn getQueue
             * @brief 
             * retrieves queue based upon the relative direction.
             * 
             * @param direction queue that should be listened too.
             * @return queue
             */
            queue<Event>* getQueue(MSPDIRECTION direction);

            /**
             * @fn getLock
             * @brief
             * Gets a lock given a direction.
             */
            mutex* getLock(MSPDIRECTION direction);
        
            void setQueue(MSPDIRECTION direction, queue<Event>* queue, mutex* lock) {
                _queues.emplace(direction, queue);
                _locks.emplace(direction, lock);
            }

        private:

            unordered_map<MSPDIRECTION, queue<Event>*>       _queues;     // events to be sent to user interface
            unordered_map<MSPDIRECTION, mutex*>              _locks;      // lock for UI queue
    };
}

#endif // RRQUEUE_HPP