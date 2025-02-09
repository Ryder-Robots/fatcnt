#include <fatcnt/events/msp104/msp104_ctl_in.hpp>

using namespace rrobot;

/*
 * elements for matrix are set as follows:
 *
 * s,0 = roll       -1:1
 * s,1 = pitch      -1:1
 * s,2 = yaw        -1:1
 * s,4 = throttle    0:1000
 * s,5 = aux1,      undefined
 * s,6 = aux2,      undefined
 * s,7 = aux3       undefined
 * s,8 = aux4       undefined
 * 
 * where state represents a series of inbound events starting at 0..n,  this is designed to influence inbound 
 * state of the drone.  For the purposes of the initial driver it is assumed that only one state '0' os accepted.
 */
Event* Msp104Ctl::serialize(vector<uint8_t> mtx) {
    msp_motor* payload = new msp_motor();
    payload->set_roll(_outbound.scale(mtx[0]));
    payload->set_pitch(_outbound.scale(mtx[1]));
    payload->set_yaw(_outbound.scale(mtx[2]));
    payload->set_throttle(_outbound.scale(mtx[3]));
    payload->set_aux1(_outbound.scale(mtx[4]));
    payload->set_aux2(_outbound.scale(mtx[5]));
    payload->set_aux3(_outbound.scale(mtx[6]));
    payload->set_aux4(_outbound.scale(mtx[7]));
    return new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_OUT, payload);
}

std::vector<uint8_t> Msp104Ctl::deserialize(Event* event) {
    msp_motor mspMotor = event->getPayload<msp_motor>();
    std::vector<uint8_t> v = {
        _inbound.scale(mspMotor.get_roll()),
        _inbound.scale(mspMotor.get_pitch()),
        _inbound.scale(mspMotor.get_yaw()),
        _inbound.scale(mspMotor.get_throttle()),
        _inbound.scale(mspMotor.get_aux1()),
        _inbound.scale(mspMotor.get_aux2()),
        _inbound.scale(mspMotor.get_aux3()),
        _inbound.scale(mspMotor.get_aux4())
    };
    return v;
}