#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

using namespace rrobot;

Event* Msp104Serializer::deserialize(msp_motor event) {
    msp_set_motor_hbridge *payload = new msp_set_motor_hbridge();

    if (event.get_throttle() >= 0) {
        payload->set_motor1(_ratio.scale(event.get_throttle()));
        payload->set_motor2(_ratio.scale(event.get_throttle()));
        if (event.get_yaw() < 1) {
            payload->set_in(0b1001);   
        } else if (event.get_yaw() > 1) {
            payload->set_in(0b0110);
        } else {
            payload->set_in(0b0101);
        }
    } else {
        payload->set_motor1(_ratio.scale(event.get_throttle()) * -1);
        payload->set_motor2(_ratio.scale(event.get_throttle()) * -1);
        if (event.get_yaw() < 1) {
            payload->set_in(0b0110);   
        } else if (event.get_yaw() > 1) {
            payload->set_in(0b1001);
        } else {
            payload->set_in(0b0101);
        }
    }
    return new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN, payload);
}

msp_motor Msp104Serializer::serialize(Event* event) {
    msp_motor motor;
    return motor;
}