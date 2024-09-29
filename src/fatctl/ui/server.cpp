/**
 * @brief concret implementation of thre server.
 *
 * The flow for this service is:
 *
 * 1. recieve request
 * 2. Upgrade connection to websocket (if conditions are met)
 * 3. Communicate with the UI.
 */

#include "server.hpp"

using namespace std;
using namespace rrobot;
using json = nlohmann::json;

json PAGE_INVALID_WEBSOCKET_REQUEST = {
    {"success", "false"},
    {"payload", ""},
    {"error", {{"code", MHD_HTTP_BAD_REQUEST}, {"message", "Invalid websocket request"}}}};


/**
 * Receives messages from the TCP/IP socket actions, and sends observations.
 *
 * @param cls The connected user
 * @return    Always NULL
 */
static void *robot_send_recieve(void *cls) {
    rrobot::rr_state_c *RR_STATE;
    RR_STATE->get_observers();
    return NULL;
}

void fatcnt_server::create() {
}

void fatcnt_server::close() { }
