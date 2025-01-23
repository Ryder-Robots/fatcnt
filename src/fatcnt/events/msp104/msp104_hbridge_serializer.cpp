#include <fatcnt/events/msp104/msp104_hbridge_serializer.hpp>

using namespace rrobot;

/*
 *  implementation of deserialization, for this implementation only implented two directions.
 */
Event* Msp104Serializer::deserialize(std::vector<uint8_t> m) {
    // For land drone the only concern is yaw and throttle.
    _pitch = _ratioD.scale(m[1]);
    _yaw = _ratioD.scale(m[2]);
    _throttle = _ratioTd.scale(m[3]);

    uint8_t in = (_pitch >= 0) ? EN_FORWARD: EN_BACKWARD;
    if (_yaw > 0) {
        in = EN_RIGHT;
    } else if (_yaw < 0) {
        in = EN_LEFT;
    }

    msp_set_motor_hbridge *payload = new msp_set_motor_hbridge();
    payload->set_in(in);
    payload->set_motor1(_throttle);
    payload->set_motor2(_throttle);
    payload->set_motor3(0);
    payload->set_motor4(0);

    return new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN, payload);
}

std::vector<uint8_t>  Msp104Serializer::serialize(Event* event) {
    
    std::vector<uint8_t> m {
        _ratioS.scale(0),
        _ratioS.scale(_pitch),
        _ratioS.scale(_yaw),
        _ratioTs.scale(_throttle),
        _ratioS.scale(0),
        _ratioS.scale(0),
        _ratioS.scale(0),
        _ratioS.scale(0)
    };
    return m;
}