#include "statefactory.hpp"

using namespace rrobot;

State* StateFactory::createState(Environment environment, vector<MSPDIRECTION> directions) {

    RrQueues* queues = new RrQueues(
        environment.getQueues().getLimit(),
        chrono::milliseconds(environment.getQueues().getThreadWaitTime()),
        chrono::milliseconds(environment.getQueues().getThreadProcessTime())
    );

    State* state = new State(queues);

    for (auto direction : directions) {
        mutex* lock = new mutex();
        queue<Event*>* q = new queue<Event*>();
        queues->setQueue(direction, q, lock);
    }

    return state; 
}