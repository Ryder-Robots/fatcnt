#include "rrqueues.hpp"

using namespace rrobot;

queue<Event>* RrQueues::getQueue(MSPDIRECTION direction) {
    if (_queues.find(direction) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    return _queues.at(direction); 
}

mutex* RrQueues::getLock(MSPDIRECTION direction) {
    if (_locks.find(direction) == _locks.end()) {
        throw QueueDoesNotExit("could not find lock for direction");
    }
    return _locks.at(direction);
}