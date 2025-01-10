#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

using namespace rrobot;

/*
 *  implementation of deserialization, for this implementation only implented two directions.
 */
Event* Msp104Serializer::deserialize(std::vector<uint8_t> m) {
    // For land drone the only concern is yaw and throttle.
    float yaw = _ratioD.scale(m[2]);
    float throttle = _ratioD.scale(m[3]);

    uint16_t motor1 = T(throttle), motor2 = T(throttle), motor3 = 0, motor4 = 0;
    uint8_t in = 0;

    if (throttle < 0) {
        // Controller is moved right
        if (yaw == 0) {
            in = 0b0101;
        } else if (yaw > 0) {
            in = 0b0110;
        } else {
            in = 0b1001;
        }
    } else {
        if (yaw == 0) {
            in = 0b1010;
        } else if (yaw > 0) {
            in = 0b1001;
        } else {
            in = 0b0110;
        }
    }
    msp_set_motor_hbridge *payload = new msp_set_motor_hbridge();
    payload->set_in(in);
    payload->set_motor1(motor1);
    payload->set_motor1(motor2);
    payload->set_motor1(motor3);
    payload->set_motor1(motor4);

    return new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN, payload);
}

std::vector<uint8_t>  Msp104Serializer::serialize(Event* event) {
    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();
    float yaw = 0, throttle = 0;

    if (payload.get_in() ==  0b0110) {
            yaw = 0.5;
            throttle = payload.get_motor1() / 1000;
    } else if (payload.get_in() ==  0b1001) {
        throttle = payload.get_motor1() / 1000;
        yaw = -0.5;
    } else if (payload.get_in() == 0b1010) {
        yaw = -0.5;
        throttle = (payload.get_motor1() / 1000) * -1;
    } else if (payload.get_in() == 0b0110) {
        yaw = 0.5;
        throttle = (payload.get_motor1() / 1000) * -1;
    }
    
    std::vector<uint8_t> m;
    m.clear();
    m.reserve(8);
    m.push_back(_ratioS.scale(0));
    m.push_back(_ratioS.scale(0));
    m.push_back(_ratioS.scale(yaw));
    m.push_back(_ratioS.scale(throttle));
    m.push_back(_ratioS.scale(0));
    m.push_back(_ratioS.scale(0));
    m.push_back(_ratioS.scale(0));
    m.push_back(_ratioS.scale(0));
    return m;
}