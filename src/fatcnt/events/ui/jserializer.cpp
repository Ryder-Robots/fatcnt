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
        j.contains("command") && (_validMspCommands.find(j["command"]) != _validMspCommands.end()) &&
        j.contains("payload")
    
    )) {
        throw MissingRequiredAttributeException("command and payload are required attributes");
    }

    RrpCuratorAbstract* curator = _curatorMap.at(_validMspCommands.at(j["command"]));
    json payload = j["payload"];
    return curator->deserialize(payload);
 }