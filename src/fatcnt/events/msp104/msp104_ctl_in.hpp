#ifndef MSP104_CTL_IN
#define MSP104_CTL_IN

#include <stdint.h>
#include <vector>
#include <fatcnt/math/ratio.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_motor.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/events/serializer.hpp>

namespace rrobot {

    /**
     * @class Msp104Ctl
     * @brief
     * accepts input from PS4 controller (ranges -1:1) and sclaes it to AI ranges (0:255).
     * 
     * Handler will pass in MSP104 event, this is converted to features for dlib matrix, which
     * can be used to produce input data for training, or for a prediction depending on mode that
     * handler is using.
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
        Ratio<float, uint8_t> _inbound = Ratio<float, uint8_t>(-1, 1, 0, 255);
        Ratio<uint8_t, float> _outbound = Ratio<uint8_t, float>(0, 255, -1, 1);
    };
}

#endif