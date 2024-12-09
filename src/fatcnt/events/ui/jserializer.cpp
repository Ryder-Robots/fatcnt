#include "jseralizer.hpp"

using namespace rrobot;

json Jseralizer::serialize(Event* event) {
    RrpCuratorAbstract* curator = _curatorMap.at(event->getCommand());
    return curator->serialize(event);
}


//TODO: This routine needs to change,  if payload is not defined, then it could be a request, so we change the 
// the direction.
 Event* Jseralizer::deserialize(json j) {
    if(!(
        j.contains("command") && (_validMspCommands.find(j["command"]) != _validMspCommands.end())
    )) {
        throw MissingRequiredAttributeException("command is required attributes");
    }

    Event* event = nullptr;
    RrpCuratorAbstract* curator = _curatorMap.at(_validMspCommands.at(j["command"]));
    if (j.contains("payload")) {   
        json payload = j["payload"];
        event = curator->deserialize(payload);
    } else {
        event = curator->deserialize(MSPDIRECTION::EXTERNAL_IN);
    }

    return event;
 }