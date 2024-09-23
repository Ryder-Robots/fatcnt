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

using namespace rrobot;


/**
 * @fn access_handler
 * @brief entry point to handle access for user input.
 * @param *cls 
 */
static enum MHD_Result access_handler(void *cls,
        struct MHD_Connection *connection,
        const char *url,
        const char *method,
        const char *version,
        const char *upload_data,
        size_t *upload_data_size,
        void **req_cls) 
{
    static int aptr;
    
    if (0 != strcmp(method, "GET")) {
        // Unexpected method
        return MHD_NO;  
    }

    if (&aptr != *req_cls) {
    /* do never respond on first call */
    *req_cls = &aptr;
    return MHD_YES;
    }


    struct MHD_Response *response;
    response = MHD_create_response_from_buffer_static (strlen (PAGE_NOT_FOUND),
                                                PAGE_NOT_FOUND);
    int ret = MHD_queue_response (connection,
                        MHD_HTTP_NOT_FOUND,
                        response);
    MHD_destroy_response (response);
    return (MHD_Result) ret;
}

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
    try {
        _ws = MHD_start_daemon (MHD_OPTS, PORT, NULL, NULL, &access_handler, NULL, MHD_OPTION_CONNECTION_TIMEOUT, TIMEOUT, MHD_OPTION_END);
    } catch (const std::exception& e) {
        //TODO: throw exception this is fatal.
        std::cout << "Exception " << e.what() << "\n";
    }
}


void fatcnt_server::close() {
    MHD_stop_daemon (_ws);
}