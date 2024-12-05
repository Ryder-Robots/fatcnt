#ifndef JSERIALIZER_HPP
#define JSERIALIZER_HPP

#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/protocols/common/curators/rrp/rrpcurator.hpp>
#include <fatcnt/protocols/common/mspversion.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

using json = nlohmann::json;

namespace rrobot {
    class Jseralizer : public Serializer<json> {
        public:
        json serialize(Event* event) override;
        Event* deserialize(json j) override;

        /*
         * Use constructor to maintain state.
         */
        Jseralizer() {
            VALID_MSPCOMMANDS;
            _validMspCommands = VALID_MSPCOMMANDS_KEYS;
            CURATOR_MAP_INIT;
            _curatorMap = CURATOR_MAP_KEYS;
        }


        private:
        unordered_map<string, MSPCOMMANDS> _validMspCommands;
        unordered_map<MSPCOMMANDS, RrpCuratorAbstract*>  _curatorMap;
    };
}

#endif // JSERIALIZER_HPP