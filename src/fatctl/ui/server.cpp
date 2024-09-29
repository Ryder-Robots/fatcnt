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

bool fatcnt_server::is_valid(struct MHD_Connection *connection, const char *version) {
    bool is_valid = true;
    const char *value = NULL;
    char sec_websocket_accept[29];

    /* check whether an websocket upgrade is requested */
    if (MHD_WEBSOCKET_STATUS_OK != MHD_websocket_check_http_version(version)) {
        is_valid = false;
    }
    value = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONNECTION);
    if (MHD_WEBSOCKET_STATUS_OK != MHD_websocket_check_connection_header(value)) {
        is_valid = false;
    }
    value = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_UPGRADE);
    if (MHD_WEBSOCKET_STATUS_OK != MHD_websocket_check_upgrade_header(value)) {
        is_valid = false;
    }
    value = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_SEC_WEBSOCKET_VERSION);
    if (MHD_WEBSOCKET_STATUS_OK != MHD_websocket_check_version_header(value)) {
        is_valid = false;
    }
    value = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_SEC_WEBSOCKET_KEY);
    if (MHD_WEBSOCKET_STATUS_OK != MHD_websocket_create_accept_header(value, sec_websocket_accept)) {
        is_valid = false;
    }

    return is_valid;
}

/**
 * @fn access_handler
 * @brief entry point to handle access for user input.
 * @param *cls
 */
enum MHD_Result fatcnt_server::access_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                              const char *method, const char *version, const char *upload_data,
                                              size_t *upload_data_size, void **req_cls) {
    static int aptr;
    int ret = MHD_NO;
    struct MHD_Response *response;
    char sec_websocket_accept[29];

    if (0 != strcmp(method, "GET")) {
        // Unexpected method
        return MHD_NO;
    } else if (&aptr != *req_cls) {
        /* do never respond on first call */
        *req_cls = &aptr;
        return MHD_YES;
    } else if (0 != strcmp(url, CONNECTION_URL)) {
        // invalid response.
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);

    } else if (is_valid(connection, version)) {
        response = MHD_create_response_for_upgrade(&upgrade_handler, NULL);
        MHD_add_response_header(response, MHD_HTTP_HEADER_UPGRADE, "websocket");
        MHD_add_response_header(response, MHD_HTTP_HEADER_SEC_WEBSOCKET_ACCEPT, sec_websocket_accept);
        ret = MHD_queue_response(connection, MHD_HTTP_SWITCHING_PROTOCOLS, response);
        MHD_destroy_response(response);
    } else {
        /* return error page */
        response = MHD_create_response_from_buffer_static(PAGE_INVALID_WEBSOCKET_REQUEST.dump().length(),
                                                          PAGE_INVALID_WEBSOCKET_REQUEST.dump().c_str());
        ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
    }
    return static_cast<MHD_Result>(ret);
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
        _ws = MHD_start_daemon(MHD_OPTS, PORT, NULL, NULL, &access_handler, NULL, MHD_OPTION_CONNECTION_TIMEOUT,
                               TIMEOUT, MHD_OPTION_END);
    } catch (const std::exception &e) {
        // TODO: throw exception this is fatal.
        std::cout << "Exception " << e.what() << "\n";
    }
}

void fatcnt_server::close() { MHD_stop_daemon(_ws); }

void fatcnt_server::upgrade_handler(void *cls, struct MHD_Connection *connection, void *req_cls, const char *extra_in,
                                    size_t extra_in_size, MHD_socket fd, struct MHD_UpgradeResponseHandle *urh) {
    pthread_t pt;
    try {
        string s_extra_in = "";
        if (extra_in != NULL) {
            s_extra_in = extra_in;
        }
        connected_user *cu = new connected_user(fd, urh, s_extra_in);

        if (0 != pthread_create(&pt, NULL, &rr_env_communicator, cu)) abort();
        pthread_detach(pt);

        // validate the username and password at this point.

    } catch (const std::exception &e) {
        struct MHD_Response *response = MHD_create_response_from_buffer_static(
            PAGE_INVALID_WEBSOCKET_REQUEST.dump().length(), PAGE_INVALID_WEBSOCKET_REQUEST.dump().c_str());
        MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
        return;
    }
}

/**
 * Control the drone from this point, the user has been authenticated, so this should be considred safe.
 */
void *fatcnt_server::rr_env_communicator(void *cls) {
    connected_user *cu = static_cast<connected_user *>(cls);
    struct MHD_WebSocketStream *ws;

    // create websocket,  this will allow communication.
    int result = MHD_websocket_stream_init(&ws, MHD_WEBSOCKET_FLAG_SERVER | MHD_WEBSOCKET_FLAG_NO_FRAGMENTS, 0);
    cu->set_ws(ws);
    if (MHD_WEBSOCKET_STATUS_OK != result) {
        MHD_upgrade_action(cu->get_urh(), MHD_UPGRADE_ACTION_CLOSE);
        return  static_cast<void *>(cu);
    }

    return static_cast<void *>(cu);
}

/*
 */
json *fatcnt_server::recieve_data(connected_user *cu, char *buf_, size_t bufsz_) {
    string s = "";
    size_t buf_offset = 0;

    size_t bufsz = BUFSIZ;
    char *buf = static_cast<char *>(malloc(BUFSIZ));

    while (buf_offset < bufsz) {
        size_t new_offset = 0;
        char *frame_data = NULL;
        size_t frame_len = 0;

        int status = MHD_websocket_decode(cu->get_ws(), buf, bufsz, &new_offset, &frame_data, &frame_len);

        if (status < 0) {
            // Error has occurred.
            MHD_websocket_free(cu->get_ws(), frame_data);
            return &PAGE_INVALID_WEBSOCKET_REQUEST;
        }
        buf_offset += new_offset;
        if (status > 0) {
            switch (status) {
                case MHD_WEBSOCKET_STATUS_TEXT_FRAME:
                case MHD_WEBSOCKET_STATUS_BINARY_FRAME:
                    cout << "I get here";
                    break;
            }
        }
    }

    return new json();
}