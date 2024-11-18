#ifndef ENVIRONMENTPROCESSOR_HPP
#define ENVIRONMENTPROCESSOR_HPP

#include <boost/program_options.hpp>
#include <fatcnt/environment/environment.hpp>

namespace rrobot {
    class EnviromentProcessor {
        public:

            /**
             * @fn createEnvironment
             * @brief
             * Creates environment, 
             *     - environment variables override manifest settings
             *     - command line options override environment variables
             */
            Environment createEnvironment(string manifest, boost::program_options po);
    };
}

#endif // ENVIRONMENTPROCESSOR_HPP