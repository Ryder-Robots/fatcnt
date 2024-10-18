#ifndef NN_HPP
#define NN_HPP

#include <pthread.h>
#include "fatctl/environment/state.hpp"

namespace rrobot {

/**
 * @brief responsible for connecting neural network with hardware. All commands either from UI, or 
 * re-enforcement learner must go through teh nuerel network.
 */
class nn {
    public:
    
    /**
     * @fn handle_operation
     * @brief handles inbound operations, this is linked to the environment and handler for the environment.
     */
    static void *handle_operations(void *state_in);

    pthread_t init(rr_state_c *state);

};
}

#endif