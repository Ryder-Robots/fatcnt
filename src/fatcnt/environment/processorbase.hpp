#ifndef PROCESSORBASE_HPP
#define PROCESSORBASE_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

using namespace std;
using json = nlohmann::json;

namespace rrobot {
    class ProcessorBase {
        protected:
            static bool verify(json manifest, std::vector<string> keys, string container);
    };
}

#endif // PROCESSORBASE_HPP