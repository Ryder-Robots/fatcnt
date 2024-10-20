#ifndef SKULD001_HANDLER_HPP
#define SKULD001_HANDLER_HPP

#include <string>
#include "fatctl/environment/handlers/handler.hpp"


using namespace std;

/**
 * @brief interacts with drone,  this is a SKULD001 that uses a H-Bridge.
 */
namespace rrobot {
    class skuld001_handler : public rr_handler {
    public:
        /**
         * @fn init
         * @brief perform any initlization.
         */
        void init() override;

        /**
         * @fn send_data
         * @brief perform actions that are sent.
         */
        int send_action(json action) override;

        /**
         * @fn recieve_observation
         * @brief sends an observation back to UI,  note that for SKULD001 this does nothing.
         */
        json recieve_observation() override;

        /**
         * @fn ~skuld001_handler
         * @brief deconstruct.
         */
        ~skuld001_handler();

    private:
        void teardown();
        void drivewheels(int in1, int in2, int in3, int in4, int ena, int enb);
        bool validate(json action);

        // L298H pin layout.
        rr_pi_pin_layout _pin_layout = rr_pi_pin_layout();
        const int _in1 = _pin_layout.get_gpio_pin23();
        const int _in2 = _pin_layout.get_gpio_pin24();
        const int _in3 = _pin_layout.get_gpio_pin27();
        const int _in4 = _pin_layout.get_gpio_pin17();
        const int _ena = _pin_layout.get_pwm0();
        const int _enb = _pin_layout.get_pwm1();
        const int _multiplier = 1000;
    };
}

#endif // SKULD001_HANDLER_HPP