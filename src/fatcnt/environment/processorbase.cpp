#include "processorbase.hpp"

using namespace rrobot;

using json = nlohmann::json;

bool ProcessorBase::verify(json manifest, vector<string> keys, string container) {
    if (!manifest.contains(container)) {
        throw MissingRequiredAttributeException("missing required attribute for " + container);
    }
    for (auto key : keys) {
        if (!manifest[container].contains(key)) {
            throw MissingRequiredAttributeException("missing required attribute " + key + " for " + container);
        }
    }
    return true;
}