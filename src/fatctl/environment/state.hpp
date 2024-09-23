/*
 */

#ifndef STATE_HPP
#define STATE_HPP

/**
 * @brief special shared object that will be used to transmit state to and from UI, Nueral networks, RL, and environment.
 */
namespace rrobot {
    class rr_state_c {
        public:
            rr_state_c(){}

            // TODO: this should do something
            void get_observers() {}
    };
}

extern rrobot::rr_state_c *RR_STATE; 

#endif
