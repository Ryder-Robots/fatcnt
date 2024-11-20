#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include <fatcnt/processorbase.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/environment/environment.hpp>

namespace rrobot {
    class StateFactory : ProcessorBase {
        public:
            static State* createState(Environment environment);
    };
}

#endif // STATEFACTORY_HPP