#ifndef MSP104_HBRIDGE_SERIALIZER
#define MSP104_HBRIDGE_SERIALIZER

#include <algorithm>
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
     * Roll: Rotation around the X-axis.
     *   Visualize this as tilting the drone to the left or right. If you imagine a cylinder 
     *   representing the drone, rolling would rotate it along its length.
     * Pitch: Rotation around the Y-axis.
     *   This is akin to nodding your head up and down. For a drone, pitching forward means 
     *   the front of the drone tilts downwards.
     * Yaw: Rotation around the Z-axis.
     *   This represents turning left or right without changing altitude. Picture turning your 
     *   head side to side.
     * Throttle:
     *    Throttle controls altitude and is not a rotational movement but rather affects the Z 
     *    position in the coordinate system. Increasing throttle raises the drone vertically, while 
     *    decreasing it lowers the drone.
     * 
     * For the purposes of a land drone, throttle is the accelration or voltage sent to the 
     * hbridge, 
     * 
     * Yaw will set IN1, IN2, IN3, IN4 which controls polarity of the engines, for example IN1 = HIGH, IN2=LOW
     * will rotate engines in a specific dirrection, giving it throttle.
     * 
     * PS4 configuration:
     * 
     *    axisYL controls Throttle
     *    axisXR controls Yaw
     */
    class Msp104Serializer : public Serializer<msp_motor> {
        public:
        /**
         * @fn deserialize
         * @brief
         * After recieving an msp_motor event convert to hbridge event.
         */
        Event* deserialize(msp_motor event) override;
        msp_motor serialize(Event* event) override;

        private:
            Ratio<uint16_t, uint8_t> _ratio = Ratio<uint16_t, uint8_t>(0, 255, -1, 1);
    };
}


#endif // MSP104_HBRIDGE_SERIALIZER
