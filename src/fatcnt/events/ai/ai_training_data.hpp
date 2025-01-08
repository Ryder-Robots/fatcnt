#ifndef AI_TRAINING_DATA_HPP
#define AI_TRAINING_DATA_HPP

#include <vector>
#include <stdint.h>
#include <fstream>
#include <string>
#include <dlib/dnn.h>
#include <dlib/logger.h>
#include <filesystem>
#include <cstdlib>
#include <lazycsv.hpp>
#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/environment/environment.hpp>

using namespace dlib;

namespace rrobot {

    /**
     * @class AiGenerateData
     * @brief
     * Given inbound envets append or create label and test data.
     */
    class AiGenerateData {
        public:

        AiGenerateData(Environment *env);
        ~AiGenerateData();

        /**
         * @fn generate
         * @brief
         * Given inbound vector, and label append or create label files.
         * 
         * @param training  vector representing a single use case.
         * @param label  expected result of training data.
         */    
        void generate(std::vector<uint8_t> training, std::vector<uint8_t> label);

        /**
         * @fn getGenisis
         * @brief
         * return first index within the training set.
         */
        uint64_t getGenesis();

        /**
         * @fn retrieveTraining
         * @brief
         * returns trainging data from position idx, until vector size is count, or till end of training data, whichever 
         * is true.  idx is updated to be the last index and vector[count - 1]
         */
        matrix<std::vector<uint8_t>> retrieveTraining(uint64_t* idx, size_t count);

        /**
         * @fn retrieveLabels
         * @brief
         * retrieve data labels for index starting at "start" parameter and ending at "end" parameter.
         * @param trainingSet name of the training set to retrieve
         * @param start beinging index
         * @param end last index in training set.
         */
        matrix<std::vector<uint8_t>> retrieveLabels(uint64_t start, uint64 end);

        private:
        uint64_t _idx = 0;  // unique index for each event
        ofstream _outstream_data;   // vector provided to AI DNN
        ofstream _outstream_labels; // expected result of vector
        std::string _data_fname;
        std::string _label_fname;
    };
}

#endif // AI_TRAINING_DATA_HPP