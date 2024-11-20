#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include <vector>
#include <queue>
#include <fatcnt/processorbase.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>

using namespace std;

namespace rrobot {
    class StateFactory : ProcessorBase {
        public:
            static State* createState(Environment environment, vector<MSPDIRECTION> directions);
    };
}

#endif // STATEFACTORY_HPP