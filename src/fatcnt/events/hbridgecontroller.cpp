#include "hbridgecontroller.hpp"

using namespace rrobot;
using namespace std;

void HbridgeController::init(StateIface *state, Environment* environment) {
    EventHandler::init(state->getQueues(), RRP_QUEUES::MICROCONTROLLER, RRP_QUEUES::CATEGORIZER, environment);
}

void HbridgeController::setUp() {
    wiringPiSetupGpio();
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);
    pinMode(_ena, PWM_OUTPUT);
    pinMode(_enb, PWM_OUTPUT);
    reset();
}

void HbridgeController::reset() {
   digitalWrite(_in1, LOW);
   digitalWrite(_in2, LOW);
   digitalWrite(_in3, LOW);
   digitalWrite(_in4, LOW);
   pwmWrite(_ena, 0);
   pwmWrite(_enb, 0);
}


bool  HbridgeController::consume(Event* event, StateIface* state) {
    _available = false;
    msp_set_motor_hbridge payload = event->getPayload<msp_set_motor_hbridge>();
    int ena = std::clamp(static_cast<int>(payload.get_motor1()),MIN_VOLTAGE, MAX_VOLTAGE);
    int enb = std::clamp(static_cast<int>(payload.get_motor2()),MIN_VOLTAGE, MAX_VOLTAGE);

    uint8_t in = payload.get_in();
    int in1 = ((in & IN1_ON) == IN1_ON)? HIGH : LOW;
    int in2 = ((in & IN2_ON) == IN2_ON)? HIGH : LOW;
    int in3 = ((in & IN3_ON) == IN3_ON)? HIGH : LOW;
    int in4 = ((in & IN4_ON) == IN4_ON)? HIGH : LOW;

    digitalWrite(_in1, in1);
    digitalWrite(_in2, in2);
    digitalWrite(_in3, in3);
    digitalWrite(_in4, in4);

    pwmWrite(_ena, ena);
    pwmWrite(_enb, enb);

    // indicates that the produce should produce return result.
    _trigger = true;
    return true;
}

Event* HbridgeController::produce(StateIface* state) {
     _trigger = false;

    digitalRead(_in1);
    digitalRead(_in2);
    digitalRead(_in3);
    digitalRead(_in4);

    analogRead(_ena);
    analogRead(_enb);

     // code goes in here
     _available = true;

    msp_set_motor_hbridge* payload = new msp_set_motor_hbridge();
    Event* event = new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_OUT, payload);
    return event;
}