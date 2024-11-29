#include "jseralizer.hpp"

using namespace rrobot;

Event* Jseralizer::serialize(json event) {
    if (!(event.contains("action") && event.contains("payload"))) {
        throw MissingRequiredAttributeException("action and payload are required acttributes");
    }

    if (VALID_MSPCOMMANDS_KEYS.find(event["action"]) == VALID_MSPCOMMANDS_KEYS.end()) {
        throw InvalidCommand(event["action"] + " is an unknown command");
    }
}