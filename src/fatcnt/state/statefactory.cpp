#include "statefactory.hpp"

using namespace rrobot;

State* StateFactory::createState(Environment environment, vector<RRP_QUEUES> queuesNames) {

    RrQueues* queues = new RrQueues(
        environment.getQueues().getLimit(),
        chrono::milliseconds(environment.getQueues().getThreadWaitTime()),
        chrono::milliseconds(environment.getQueues().getThreadProcessTime())
    );

    for (auto queueName : queuesNames) {
        mutex* lock = new mutex();
        queue<Event*>* q = new queue<Event*>();
        queues->setQueue(queueName, q, lock);
    }

    State* state = new State(queues);
    
    return state; 
}