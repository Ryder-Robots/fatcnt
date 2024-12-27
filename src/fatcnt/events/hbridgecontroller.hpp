#ifndef HBRIDGECONTROLLER_HPP
#define HBRIDGECONTROLLER_HPP

#include <algorithm>
#include <wiringPi.h>
#include <fatcnt/state/stateiface.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_set_motor_hbridge.hpp>
#include "eventhandler.hpp"

namespace rrobot {

/**
 * @class HbridgeController
 * @brief
 * For 2 wheel drive land drones, Hbridge is accessed directly from the raspberrt pi.
 */
class HbridgeController : public EventHandler {
   public:
    HbridgeController(int in1 = 23, int in2 = 24, int in3 = 27, int in4 = 17, int ena = 12, int enb = 13)
        : _in1(in1), _in2(in2), _in3(in3), _in4(in4), _ena(ena), _enb(enb) {}

    /**
     * @fn setUp
     * @brief
     * inilizes Hbridge and WiringPi.
     */
    void setUp() override;

    /**
     * @fn consume
     * @brief
     * Accept Hbridge events, and send them to Hbridge controller.
     */
    bool consume(Event* event, StateIface* state) override;

    /**
     * @fn name
     * @brief
     * returns HbridgeController
     */
    string name() override { return "HbridgeController"; }

    /**
     * @fn produce
     * @brief
     * return current status of hbridge
     */
    Event* produce(StateIface* state) override;

    /**
     * @fn available
     * @brief
     * indicates there are events written to output.
     */
    bool available() override { return _available; }

    /**
     * @fn init
     * @brief
     * inilizes handler.
     */
    void init(StateIface* state, Environment* environment);

    void reload() override { reset(); }

    void tearDown() override { reset(); }

   private:
    void reset();

    // GPIO ping laylout.
    const int _in1;
    const int _in2;
    const int _in3;
    const int _in4;
    const int _ena;
    const int _enb;

    // transaction ID for feature list

    bool _available = false;
    bool _trigger = false;

    const int MIN_VOLTAGE = 0;
    const int MAX_VOLTAGE = 1000;

    const uint8_t IN1_ON = 1 << 0;
    const uint8_t IN2_ON = 1 << 1;
    const uint8_t IN3_ON = 1 << 2;
    const uint8_t IN4_ON = 1 << 3;
};
}  // namespace rrobot

#endif  // HBRIDGECONTROLLER_HPP