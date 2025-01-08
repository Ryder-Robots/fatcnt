#ifndef AI_TRAINING_DATA_HPP
#define AI_TRAINING_DATA_HPP

#include <vector>
#include <stdint.h>
#include <fstream>
#include <string>
#include <dlib/dnn.h>
#include <dlib/logger.h>
#include <fatcnt/environment/environment.hpp>

using namespace std;
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

        /**
         * @fn generate
         * @brief
         * Given inbound vector, and label append or create label files.
         * 
         * @param trainingSet unique title that represents this dataset, if it exists it will be appended to.
         * @param training  vector representing a single use case.
         * @param label  expected result of training data.
         */    
        void generate(string trainingSet, std::vector<uint8_t> training, std::vector<uint8_t> label);

        /**
         * @fn getGenisis
         * @brief
         * return first index within the training set.
         * @param trainingSet name of training set.
         */
        uint64_t getGenisis(string trainingSet);

        /**
         * @fn retrieveTraining
         * @brief
         * returns trainging data from position idx, until vector size is count, or till end of training data, whichever 
         * is true.  idx is updated to be the last index and vector[count - 1]
         */
        matrix<std::vector<uint8_t>> retrieveTraining(string trainingSet, uint64_t* idx, size_t count);

        /**
         * @fn retrieveLabels
         * @brief
         * retrieve data labels for index starting at "start" parameter and ending at "end" parameter.
         * @param trainingSet name of the training set to retrieve
         * @param start beinging index
         * @param end last index in training set.
         */
        matrix<std::vector<uint8_t>> retrieveLabels(string trainingSet, uint64_t start, uint64 end);

        private:
        uint64_t _idx = 0;  // unique index for each event
        ofstream _outstream_data;   // vector provided to AI DNN
        ofstream _outstream_labels; // expected result of vector
        string _training_set_path;  // path where training sets can be located.

        const string set_lbl_pfix = ".lbl.dat";
        const string set_trn_pfix = ".trn.dat";
    };
}

#endif // AI_TRAINING_DATA_HPP