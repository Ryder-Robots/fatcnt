#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include <vector>
#include <queue>
#include <condition_variable>
#include <fatcnt/state/state.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/environment/environment.hpp>

using namespace std;

namespace rrobot {
    class StateFactory  {
        public:
            /**
             * @fn createState
             * @brief
             * Creates state and queues based upon inputs.
             */
            static State* createState(Environment environment, vector<RRP_QUEUES> queues);
    };
}

#endif // STATEFACTORY_HPP