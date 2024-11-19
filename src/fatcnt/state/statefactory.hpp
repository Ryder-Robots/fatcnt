#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include <fatcnt/state/state.hpp>
#include <fatcnt/environment/environment.hpp>

namespace rrobot {
    class StateFactory {
        public:
            State createState(Environment environment);
    };
}

#endif // STATEFACTORY_HPP