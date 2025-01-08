#ifndef ENVIRONMENTPROCESSOR_HPP
#define ENVIRONMENTPROCESSOR_HPP

#include <vector>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <dlib/logger.h>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include "processorbase.hpp"

using namespace std;
using json = nlohmann::json;

namespace rrobot {
    class EnviromentProcessor : ProcessorBase {
        public:

            /**
             * @fn createEnvironment
             * @brief
             * Creates environment, 
             *     - environment variables override manifest settings
             *     - command line options override environment variables
             */
            static Environment createEnvironment(json manifest);
            static Environment* createEnvironmentRef(json manifest);
        
        private:
            static HwModel  createHwModel(json manifest);
            static RrSerial createMc(json manifest);
            static Queues   createQueues(json manifest);
            static Server   createServer(json server);
            static Logging  createLogging(json logging);
            static EaiTrainingData createAiTrainingData(json ai_training);

    };
}

#endif // ENVIRONMENTPROCESSOR_HPP