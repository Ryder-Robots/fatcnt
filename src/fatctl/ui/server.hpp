/*!
 * defines the server.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <microhttpd.h>
#include <microhttpd_ws.h>
#include <stdint.h>
#include <string.h>

#include <ext/stdio_filebuf.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "fatctl/environment/state.hpp"
#include "fatctl/ui/connected_user.hpp"

#define PAGE_NOT_FOUND "404 Not Found"
#define MHD_OPTS MHD_ALLOW_UPGRADE | MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG
#define PORT 8081
#define TIMEOUT (uint32_t)120

// Create authorization header.
#define CONNECTION_URL "/"
using json = nlohmann::json;

/**
 * @brief websocket server.
 */
namespace rrobot {
class fatcnt_server {
   public:
    /**
     *  @fn fatcnt_server
     *  @brief class constructor
     */
    fatcnt_server() : _ws(NULL) {}

    /**
     * @fn create
     * @brief creates an instance of the server.
     */
    void create();

    /**
     * @fn close
     * @brief stops the server
     */
    void close();

   protected:
    /**
     * @fn upgrade_handler
     * @brief upgrade connection from HTTP to WS.
     * @param cls
     * @param connection connection structure
     * @param req_cls
     * @param extra_in
     * @param extra_in_size in bytes of extra_in
     * @param fd file descriptor for websocket
     * @param urh upgrade response handler
     */
    static void upgrade_handler(void *cls, struct MHD_Connection *connection, void *req_cls, const char *extra_in,
                                size_t extra_in_size, MHD_socket fd, struct MHD_UpgradeResponseHandle *urh);

    static enum MHD_Result access_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                          const char *method, const char *version, const char *upload_data,
                                          size_t *upload_data_size, void **req_cls);
    
    static json* recieve_data(connected_user *cu, char *buf, size_t bufsz);

   private:
    struct MHD_Daemon *_ws;

    static bool is_valid(struct MHD_Connection *connection, const char *version);

    /**
     * @fn rr_env_communicator
     * @brief use to communicate to enivoronment, actions are recieved from the UI, and observations and transmitted
     * back to UI.
     */
    static void *rr_env_communicator(void *cu);
};
}  // namespace rrobot

#endif