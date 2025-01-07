#ifndef MSP104_CTL_IN
#define MSP104_CTL_IN

#include <dlib/dnn.h>
#include <fatcnt/events/serializer.hpp>

using namespace dlib;

namespace rrobot {

    /**
     * @class Msp104Ctl
     * @brief
     * accepts input from PS4 controller (ranges -1:1) and sclaes it to AI ranges (0:255)
     */
    class Msp104Ctl : public Serializer<matrix<uint8_t>> {

    };
}

#endif