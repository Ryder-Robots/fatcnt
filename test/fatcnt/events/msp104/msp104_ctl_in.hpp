#ifndef MSP104_CTL_IN
#define MSP104_CTL_IN

#include <stdint.h>
#include <vector>
#include <fatcnt/math/ratio.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/events/serializer.hpp>


//TODO: Deprecate this!!
namespace rrobot {

    /**
     * @class Msp104Ctl
     * @brief
     * accept commands from sensor, and to MSP104 and return predi
     */
    class Msp104Ctl : public Serializer<Event*, std::vector<uint8_t>> {
        public:
        /**
         * @fn serialize
         * @brief
         * converts DLIB matrix to MSP104 event
         */
        Event* serialize(std::vector<uint8_t>) override;

        /**
         * @fn deserialize
         * @brief
         * converts MSP104 to DLIB matrix
         */
        std::vector<uint8_t> deserialize(Event* event) override;

        private:
        Ratio<int8_t, uint8_t> _inbound = Ratio<int8_t, uint8_t>(-1, 1, 0, 255);
        Ratio<uint8_t, int8_t> _outbound = Ratio<uint8_t, int8_t>(0, 255, -1, 1);

        Ratio<uint8_t, uint16_t> _ratioTs = Ratio<uint8_t, uint16_t>(0, 255, 0, 1000);
        Ratio<uint16_t, uint8_t> _ratioTd = Ratio<uint16_t, uint8_t>(0, 1000, 0, 255);

    };
}

#endif