#ifndef PI_PIN_LAYOUT_HPP
#define PI_PIN_LAYOUT_HPP

#include <wiringPi.h>

/**
 * @brief defines common pinlayout for raspbery pi,  valid for 4B.
 */
namespace rrobot {


    class rr_pi_pin_layout {
    public:
        int get_gpio_pin5() {
            return _gpio_pin5;
        }

        int get_gpio_pin6() {
            return _gpio_pin6;
        }

        int get_gpio_pin12() {
            return _gpio_pin12;
        }

        int get_gpio_pin13() {
            return _gpio_pin13;
        }

        int get_gpio_pin17() {
            return _gpio_pin17;
        }

        int get_gpio_pin22() {
            return _gpio_pin22;
        }

        int get_gpio_pin24() {
            return _gpio_pin24;
        }

        int get_gpio_pin23() {
            return _gpio_pin23;
        }

        int get_gpio_pin25() {
            return _gpio_pin25;
        }

        int get_gpio_pin26() {
            return _gpio_pin26;
        }

        int get_gpio_pin27() {
            return _gpio_pin27;
        }

        int get_pwm0() {
            return _pwm0;
        }

        int get_pwm1() {
            return _pwm1;
        }

    private:
        // pin layout.
        const int _gpio_pin5  =  5; 
        const int _gpio_pin6  =  6;
        const int _gpio_pin17 = 17;
        const int _gpio_pin22 = 22;
        const int _gpio_pin23 = 23;
        const int _gpio_pin24 = 24;
        const int _gpio_pin25 = 25;
        const int _gpio_pin26 = 26;
        const int _gpio_pin27 = 27;

        const int _gpio_pin12 = 12;
        const int _gpio_pin13 = 13;
        const int _pwm0 = _gpio_pin12;
        const int _pwm1 = _gpio_pin13;
    };
}

#endif //PI_PIN_LAYOUT_HPP