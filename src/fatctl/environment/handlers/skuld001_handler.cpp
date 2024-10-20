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
      if (!payload[k].is_number()) {
         return false;
      }
   }
   return true;
}

/*
 * recieve an action and do something with it. The something is send voltage to wheels.
 */
int skuld001_handler::send_action(json action) {

   if (!validate(action)) {
      return 1;
   }

   json payload = json();
   action["payalod"].get_to(payload);

   float ea, eb;
   int in1, in2, in3, in4, in;
   ea = payload["ea"].template get<float>();
   eb = payload["eb"].template get<float>();
   in = payload["in"].template get<int>(); 
   payload["in"].get_to(in);

   ea *= _multiplier;
   eb *= _multiplier;

   in1 = in & 0b0001;
   in2 = in & 0b0010;
   in3 = in & 0b0100;
   in4 = in & 0b1000;

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