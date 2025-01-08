#ifndef MSP104_HBRIDGE_SERIALIZER
#define MSP104_HBRIDGE_SERIALIZER

#include <dlib/dnn.h>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/math/ratio.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_set_motor_hbridge.hpp>

using namespace dlib;

namespace rrobot {

    /**
     *  @class Msp104Serializer
     *  @brief
     *  Converts inbound MSP 104 event to hbridge event.
     * 
     */ 
    class Msp104Serializer : public Serializer<matrix<uint8_t>, Event*> {
        public:
        /**
         * @fn deserialize
         * @brief
         * After recieving an msp_motor event convert to hbridge event.
         */
        Event* deserialize(matrix<uint8_t>) override;
        matrix<uint8_t> serialize(Event* event) override;

        private:
            Ratio<uint8_t, float> _ratioD = Ratio<uint8_t, float>( -1, 1, 0, 255);
            Ratio<float, uint8_t> _ratioS = Ratio<float, uint8_t>(0, 2455, -1, 1);
            uint16_t T(float i) {return static_cast<uint16_t>(((i<0)?i*-1:i)*1000);}
    };
}


#endif // MSP104_HBRIDGE_SERIALIZER
