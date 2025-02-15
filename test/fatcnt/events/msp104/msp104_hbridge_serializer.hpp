#ifndef MSP104_HBRIDGE_SERIALIZER
#define MSP104_HBRIDGE_SERIALIZER

#include <algorithm>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor_curator.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_set_motor_hbridge.hpp>
#include <fatcnt/events/Event.hpp>

namespace rrobot {

    /**
     *  @class Msp104Serializer
     *  @brief
     *  Converts inbound MSP 104 event to hbridge event.
     * 
     * TODO: May move this to robot repository.
     * 
     */ 
    class Msp104Serializer  {
        public:
        /**
         * @fn deserialize
         * @brief
         * After recieving an msp_motor event convert to hbridge event.
         */
        Event* convert(Event* event);

        private:
            const uint8_t EN_BACKWARD =  0b0101;
            const uint8_t EN_FORWARD =  0b1010;
            const uint8_t EN_LEFT = 0b0110;
            const uint8_t EN_RIGHT = 0b1001;
    };
}


#endif // MSP104_HBRIDGE_SERIALIZER
