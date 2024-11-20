#include "statefactory.hpp"

using namespace rrobot;

State StateFactory::createState(Environment environment) {

    RrQueues queues = RrQueues(
        environment.getQueues().getLimit(),
        chrono::milliseconds(environment.getQueues().getThreadWaitTime()),
        chrono::milliseconds(environment.getQueues().getThreadProcessTime())
    );

    State state = State(queues);
    return state; 
}