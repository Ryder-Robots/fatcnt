#ifndef ENVIRONMENTPROCESSOR_HPP
#define ENVIRONMENTPROCESSOR_HPP

#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <dlib/logger.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>

#include <fatcnt/environment/environment.hpp>
#include <fatcnt/exceptions/exceptions.hpp>

using namespace std;
using json = nlohmann::json;
namespace po = boost::program_options;

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
            static Environment createEnvironment(string manifest, po::variables_map vm);
        
        private:
            static HwModel createHwModel(json manifest);
            static RrSerial createMc(json manifest);
    };
}

#endif // ENVIRONMENTPROCESSOR_HPP