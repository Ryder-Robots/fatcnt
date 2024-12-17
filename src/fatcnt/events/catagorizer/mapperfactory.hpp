#ifndef MAPPFACTORY_HPP
#define MAPPFACTORY_HPP

#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/stateiface.hpp>
#include "catgorizermapper.hpp"
#include "ldsqu001mapper.hpp"

namespace rrobot {
    class MapperFactory {
    public:
        /**
         * @fn getMapper
         * @brief
         * Retrieve the mapper object.
         */
        static RrCatagorizerMapper* getMapper(Environment env);
    };
}

#endif // MAPPFACTORY_HPP