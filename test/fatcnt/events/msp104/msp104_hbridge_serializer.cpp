#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

using namespace rrobot;

/*
 *  implementation of deserialization, for this implementation only implented two directions.
 */
Event* Msp104Serializer::convert(Event* inbound) {
    msp_motor payload = inbound->getPayload<msp_motor>();

    uint8_t in = (payload.get_pitch() >= 0) ? EN_FORWARD: EN_BACKWARD;
    if (payload.get_yaw() > 0) {
        in = EN_RIGHT;
    } else if (payload.get_yaw() < 0) {
        in = EN_LEFT;
    }

    msp_set_motor_hbridge *payloadOut = new msp_set_motor_hbridge();
    payloadOut->set_in(in);
    payloadOut->set_motor1(payload.get_throttle());
    payloadOut->set_motor2(payload.get_throttle());
    payloadOut->set_motor3(0);
    payloadOut->set_motor4(0);

    return new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN, payloadOut);
}
