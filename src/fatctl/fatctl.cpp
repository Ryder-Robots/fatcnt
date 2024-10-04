#include "fatctl.h"

using namespace rrobot;

dlib::logger dlog_main("rr_robot_main");
int main() {
    dlog_main.set_level(dlib::LALL);

    // Create state first
    dlog_main << dlib::LINFO << "creating state";
    rrobot::rr_state_c *rr_state = new rrobot::rr_state_c();
     // perform state initlization.

    // Create UX
    dlog_main << dlib::LINFO << "starting user interface";
    fatcnt_server fs = fatcnt_server();
    fs.create(DEFAULT_PORT, rr_state);

    // get ready to recieve connections
    dlog_main << dlib::LINFO << "waiting to accept connections on port: " << DEFAULT_PORT;
    pthread_t ptid_fs = fs.rr_accept();

    // spin up nerual network, if you can.

    // keep running until we are told to shutdown.
    pthread_join(ptid_fs, NULL);
    fs.dispose();

    return 0;
}