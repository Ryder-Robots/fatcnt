#include "jseralizer.hpp"

using namespace rrobot;

json Jseralizer::serialize(Event* event) {
    RrpCuratorAbstract* curator = _curatorMap.at(event->getCommand());
    return curator->serialize(event);
}

 Event* Jseralizer::deserialize(json j) {
    if(!(
        j.contains("command") && (_validMspCommands.find("command") != _validMspCommands.end()) &&
        j.contains("payload")
    
    )) {
        throw MissingRequiredAttributeException("command and payload are required attributes");
    }

    RrpCuratorAbstract* curator = _curatorMap.at(j["command"]);
    return curator->deserialize(j["payload"]);
 }