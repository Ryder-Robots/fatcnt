#include "fatctl/environment/handlers/skuld001_handler.hpp"

using namespace rrobot;

/*
 * setup wiringPi to use the pins directly. Pin mode is GPIO. 
 */
void skuld001_handler::init() {
   wiringPiSetupGpio();

   pinMode(_in1, OUTPUT);
   pinMode(_in2, OUTPUT);
   pinMode(_in3, OUTPUT);
   pinMode(_in4, OUTPUT);
   pinMode(_ena, PWM_OUTPUT);
   pinMode(_enb, PWM_OUTPUT);

   teardown();
}

/*
 * set everything to default pin down.
 */
skuld001_handler::~skuld001_handler() {
   teardown();
}

/*
 * validate all the code.
 */
bool skuld001_handler::validate(json operation) {

   if (!operation.contains("action") || !operation.contains("payload")) {
      return false;
   }

   json action = json(), payload = json();
   operation["action"].get_to(action);
   operation["payload"].get_to(payload);

   if (!action.contains("op") || action["op"].template get<string>() != "OP_U1_MA_VCC") {
      return false;
   }
   string num_check[] = {"ea", "eb", "in"};
   for (auto k : num_check) {
      if (!payload.contains(k)) {
         return false;
      } else if (!payload[k].is_number()) {
         return false;
      }
   }
   _payload = payload;
   return true;
}

/*
 * recieve an action and do something with it. The something is send voltage to wheels.

 * TODO: Want to set a mutex on this, so we do not try and execute two commands at the same time.
 */
int skuld001_handler::send_action(json action) {

   if (!validate(action)) {
      return 1;
   }

   float ea, eb;
   int in1, in2, in3, in4, in;
   ea = _payload["ea"].template get<float>();
   eb = _payload["eb"].template get<float>();
   in = _payload["in"].template get<int>(); 

   ea *= _multiplier;
   eb *= _multiplier;

   if (ea > _multiplier || eb > _multiplier) {
      return 2;
   }

   in1 = ((in & _IN1_ON) == _IN1_ON)? HIGH : LOW;
   in2 = ((in & _IN2_ON) == _IN2_ON)? HIGH : LOW;
   in3 = ((in & _IN3_ON) == _IN3_ON)? HIGH : LOW;
   in4 = ((in & _IN4_ON) == _IN4_ON)? HIGH : LOW;

   drivewheels(in1, in2, in3, in4, ea, eb);

   return 0;
 }

 json skuld001_handler::recieve_observation() {
    return json();
 }


// private function switches motor off,  this is written into the write paper of 
// L298n chip.
void skuld001_handler::teardown() {
   digitalWrite(_in1, LOW);
   digitalWrite(_in2, LOW);
   digitalWrite(_in3, LOW);
   digitalWrite(_in4, LOW);
   pwmWrite(_ena, 0);
   pwmWrite(_enb, 0);
}

/*
 * drive the wheels, just send the values straight to the wheels.
 */
void skuld001_handler::drivewheels(int in1, int in2, int in3, int in4, int ena, int enb) {
   digitalWrite(_in1, in1);
   digitalWrite(_in2, in2);
   digitalWrite(_in3, in3);
   digitalWrite(_in4, in4);

   pwmWrite(_ena, ena);
   pwmWrite(_enb, enb);
}