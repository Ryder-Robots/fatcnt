#include "jseralizer.hpp"

using namespace rrobot;

json Jseralizer::serialize(Event* event) {
    RrpCuratorAbstract* curator = _curatorMap.at(event->getCommand());
    return curator->serialize(event);
}

//TODO: Add deserializer that allows for direction.
Event* Jseralizer::deserialize(json j) {
    return deserialize(j, MSPDIRECTION::EXTERNAL_IN);
}

Event* Jseralizer::deserialize(json j, MSPDIRECTION direction) {
    if(!(
        j.contains("command") && (_validMspCommands.find(j["command"]) != _validMspCommands.end())
    )) {
        throw MissingRequiredAttributeException("command is required attributes");
    }

    Event* event = nullptr;
    RrpCuratorAbstract* curator = _curatorMap.at(_validMspCommands.at(j["command"]));
    if (j.contains("payload")) {   
        json payload = j["payload"];
        event = curator->deserialize(payload, direction);
    } else {
        event = curator->deserialize(direction);
    }

    return event;    
}