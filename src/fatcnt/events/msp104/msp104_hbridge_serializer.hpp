#ifndef MSP104_HBRIDGE_SERIALIZER
#define MSP104_HBRIDGE_SERIALIZER

#include <vector>
#include <fatcnt/events/serializer.hpp>
#include <fatcnt/math/ratio.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_set_motor_hbridge.hpp>

namespace rrobot {

    /**
     *  @class Msp104Serializer
     *  @brief
     *  Converts inbound MSP 104 event to hbridge event.
     * 
     */ 
    class Msp104Serializer : public Serializer<std::vector<uint8_t>, Event*> {
        public:
        /**
         * @fn deserialize
         * @brief
         * After recieving an msp_motor event convert to hbridge event.
         */
        Event* deserialize(std::vector<uint8_t>) override;
        std::vector<uint8_t> serialize(Event* event) override;

        private:
            Ratio<uint8_t, uint16_t> _ratioTd = Ratio<uint8_t, uint16_t>(0, 255, 0, 1000);
            Ratio<uint8_t, float> _ratioD = Ratio<uint8_t, float>(0, 255, -1, 1);


            Ratio<uint16_t, uint8_t> _ratioTs = Ratio<uint16_t, uint8_t>(0, 1000, 0, 255);
            Ratio<float, uint8_t> _ratioS = Ratio<float, uint8_t>(-1, 1, 0, 255);

            uint8_t _yaw;
            uint16_t _throttle;
            uint8_t _pitch;

            const uint8_t EN_BACKWARD =  0b0101;
            const uint8_t EN_FORWARD =  0b1010;
            const uint8_t EN_LEFT = 0b0110;
            const uint8_t EN_RIGHT = 0b1001;
    };
}


#endif // MSP104_HBRIDGE_SERIALIZER
