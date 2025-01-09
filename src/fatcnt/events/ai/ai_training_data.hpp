#ifndef AI_TRAINING_DATA_HPP
#define AI_TRAINING_DATA_HPP

#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include <dlib/dnn.h>
#include <dlib/logger.h>
#include <filesystem>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
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
         * @fn open_write
         * @brief
         * open file for writing test data.
         * @param theading training data headings.
         * @param lheadigns label data headings.
         */
        void open_write();

        /**
         * @fn open_read
         * @brief
         * open training data for reading.
         */
        void open_read();

        /**
         * @fn close
         * @brief
         * close training data IO streams.
         */
        void close_write();

        /**
         * @fn close
         * @brief
         * close training data IO streams.
         */
        void close_read();

        /**
         * @fn write_data
         * @brief
         * Given inbound vector, and label append or create label files.
         * 
         * @param headings row headings
         * @param training  vector representing a single use case.
         * @param label  expected result of training data.
         */    
        size_t write_data(std::vector<uint8_t> training, std::vector<uint8_t> label);

        /**
         * @fn retrieveTraining
         * @brief
         * returns trainging data from position idx, until vector size is count, or till end of training data, whichever 
         * is true.  idx is updated to be the last index and vector[count - 1]
         */
        streampos retrieve_data(streampos idx, size_t count, 
        std::vector<std::vector<uint8_t>>& training, 
        std::vector<std::vector<uint8_t>>& labels);

        private:
        uint64_t _idx = 0;  // unique index for each event
        ofstream _outstream_data;   // vectors serialized for label and training
        ifstream _instream_data;
        std::string _data_fname;
    };
}

#endif // AI_TRAINING_DATA_HPP