#include "fatctl.h"

using namespace rrobot;

int main() {
    // Create state first
    rrobot::rr_state_c *RR_STATE = new rrobot::rr_state_c();

    //TODO: this should all come from an uploaded config
    // Create webserver
    fatcnt_server fs = fatcnt_server();

    // start server
    //fs.create();

    while(1) {}

    return 0;
}