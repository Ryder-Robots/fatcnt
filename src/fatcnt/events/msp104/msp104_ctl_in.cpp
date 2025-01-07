#include <fatcnt/events/msp104/msp104_ctl_in.hpp>

using namespace rrobot;

/*
 * elements for matrix are set as follows:
 *
 * s,0 = roll
 * s,1 = pitch
 * s,2 = yaw
 * s,4 = throttle
 * s,5 = aux1,
 * s,6 = aux2,
 * s,7 = aux3
 * s,8 = aux4
 * 
 * where state represents a series of inbound events starting at 0..n,  this is designed to influence inbound 
 * state of the drone.  For the purposes of the initial driver it is assumed that only one state '0' os accepted.
 */
Event* Msp104Ctl::serialize(matrix<uint8_t> mtx) {
    msp_motor* payload = new msp_motor();
    payload->set_roll(_outbound.scale(mtx(0,0)));
    payload->set_pitch(_outbound.scale(mtx(0,1)));
    payload->set_yaw(_outbound.scale(mtx(0,2)));
    payload->set_throttle(_outbound.scale(mtx(0,3)));
    payload->set_aux1(_outbound.scale(mtx(0,4)));
    payload->set_aux2(_outbound.scale(mtx(0,5)));
    payload->set_aux3(_outbound.scale(mtx(0,6)));
    payload->set_aux4(_outbound.scale(mtx(0,7)));
    return new Event(MSPCOMMANDS::MSP_MOTOR, MSPDIRECTION::EXTERNAL_OUT, payload);
}

matrix<uint8_t> Msp104Ctl::deserialize(Event* event) {
    msp_motor mspMotor = event->getPayload<msp_motor>();
    matrix<uint8_t> M(1, 8);
    M(0,0) = _inbound.scale(mspMotor.get_roll());
    M(0,1) = _inbound.scale(mspMotor.get_pitch());
    M(0,2) = _inbound.scale(mspMotor.get_yaw());
    M(0,3) = _inbound.scale(mspMotor.get_throttle());
    M(0,4) = _inbound.scale(mspMotor.get_aux1());
    M(0,5) = _inbound.scale(mspMotor.get_aux2());
    M(0,6) = _inbound.scale(mspMotor.get_aux3());
    M(0,7) = _inbound.scale(mspMotor.get_aux3());
    return M;
}