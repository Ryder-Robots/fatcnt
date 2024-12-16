#include "ldsqu001mapper.hpp"

using namespace rrobot;

vector<RRP_QUEUES> LdSqu001Mapper::queueNames() {
    vector<RRP_QUEUES> queues = {CATEGORIZER, STATUS, USER_INTERFACE, MICROCONTROLLER,};
    return queues;
}

vector<EventHandler*> LdSqu001Mapper::createEventHandlers() {
    RrStatusHandler* statusHandler = new RrStatusHandler();
    UiHandler* uiHandler = new UiHandler();

    vector<EventHandler*> eventHandlers = {};

    statusHandler->init(_state, _environment, eventHandlers);

    Jseralizer* jserializer = new Jseralizer();
    RrServer* server = new RrServer();
    uiHandler->init(server, _state, jserializer);

    eventHandlers.push_back(statusHandler);
    eventHandlers.push_back(uiHandler);

    return eventHandlers;
}

RRP_QUEUES mapQueue(Event* event) {

}