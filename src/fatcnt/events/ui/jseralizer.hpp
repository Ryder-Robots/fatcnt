#ifndef JSERIALIZER_HPP
#define JSERIALIZER_HPP

#include <unordered_map>
#include <nlohmann/json.hpp>
#include "serializer.hpp"
#include <fatcnt/events/Event.hpp>
#include <fatcnt/protocols/common/mspversion.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

using json = nlohmann::json;

namespace rrobot {
    class Jseralizer : public Serializer<json> {
        public:
        Event* serialize(json event) override;

        private:
        VALID_MSPCOMMANDS;
    };
}

#endif // JSERIALIZER_HPP