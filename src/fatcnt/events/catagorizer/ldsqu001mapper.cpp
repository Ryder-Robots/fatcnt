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
    uiHandler->init(server, _environment, _state, jserializer);

    eventHandlers.push_back(statusHandler);
    eventHandlers.push_back(uiHandler);

    return eventHandlers;
}

RRP_QUEUES LdSqu001Mapper::mapQueue(Event* eventRef) {

    if (eventRef == nullptr) {
        throw InvalidMessageSent("null event was recieved");
    }

    Event event = *eventRef;
    // send all external out to user interface.
    if (event.getDirection() == MSPDIRECTION::EXTERNAL_OUT) {
        return RRP_QUEUES::USER_INTERFACE;
    }
    RRP_QUEUES queue = RRP_QUEUES::NONE;
    switch (event.getCommand()) {
        case MSPCOMMANDS::MSP_AUTHKEY:
        case MSPCOMMANDS::MSP_IDENT:
        case MSPCOMMANDS::MSP_STATUS:
            queue = RRP_QUEUES::STATUS;
            break;
        case MSPCOMMANDS::MSP_MODE:
            if (!event.hasPayload()) {
                throw MissingRequiredAttributeException("no mode was given");
            }
            {
                msp_mode mspMode = event.getPayload<msp_mode>();
                _mode = mspMode.get_mode();
                msp_mode* payload = new msp_mode();
                payload->set_mode(_mode);
                eventRef = new Event(MSPCOMMANDS::MSP_MODE, MSPDIRECTION::EXTERNAL_OUT, payload);
            }
            queue = RRP_QUEUES::USER_INTERFACE;
            break;
        case MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE:
            if (_mode == RR_CMMODES::CMODE_MANUAL_FLIGHT) {
                queue = RRP_QUEUES::MICROCONTROLLER;
                break;
            } 
            {
                msp_error* payload = new msp_error();
                payload->set_message("unsupported command for this drone"); 
                eventRef = new Event(MSPCOMMANDS::MSP_ERROR, MSPDIRECTION::EXTERNAL_OUT, payload);
            }
            queue = RRP_QUEUES::USER_INTERFACE;
            break;
    }
    return queue;
}