#include <fatcnt/state/rrqueues.hpp>


using namespace rrobot;

queue<Event*>* RrQueues::getQueue(RRP_QUEUES direction) {
    if (_queues.find(direction) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    return _queues.at(direction);
}

mutex* RrQueues::getLock(RRP_QUEUES direction) {
    if (_locks.find(direction) == _locks.end()) {
        throw QueueDoesNotExit("could not find lock for direction");
    }
    return _locks.at(direction);
}

void RrQueues::setQueue(RRP_QUEUES direction, queue<Event*>* queue, mutex* lock) {
    _queues.emplace(direction, queue);
    _locks.emplace(direction, lock);
}

RrQueues::~RrQueues() {
    for (auto it = _queues.begin(); it != _queues.end(); ++it) {
        while(!it->second->empty()) {
            it->second->pop();
        }
        delete(it->second);
    }

    for (auto it = _locks.begin(); it != _locks.end(); ++it) {
        delete(it->second);
    }
}