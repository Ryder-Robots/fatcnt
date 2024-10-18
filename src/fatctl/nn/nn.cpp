#include "fatctl/nn/nn.hpp"

using namespace rrobot;
using namespace std;

void *nn::handle_operations(void *state_in) {
    rr_state_c *state = reinterpret_cast<rr_state_c *>(state_in);

    // TODO: need to make an exit condition here.
    while (true) {
        while (state->events_empty()) {}

        json event = state->get_event();

        // perform any pre-peocessing to nurel network here.

        // record data here.

        state->get_handler()->send_action(event);
    }
    

    return static_cast<void *>(0);
}


pthread_t nn::init(rr_state_c *state) {
    pthread_t ptid;
    pthread_create(&ptid, NULL, &nn::handle_operations, state);
    return ptid;
}