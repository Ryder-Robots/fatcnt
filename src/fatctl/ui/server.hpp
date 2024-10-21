/*!
 * defines the server.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include <unistd.h> 
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <dlib/logger.h>
#include <dlib/misc_api.h>
#include <sys/ioctl.h>

#include "fatctl/environment/state.hpp"
#include "fatctl/ui/websocket_status_code.hpp"
#include "fatctl/ui/socket_env.hpp"
#include "fatctl/ui/ux_manifest.hpp"


// Create authorization header.
#define CONNECTION_URL "/"

// TODO: These should come from common headers.  but lets code it here for now
#define RR_CMD_EXIT 0x0e

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
    fatcnt_server() {}

    /**
     * @fn create
     * @brief creates an instance of the server.
     */
    socket_env *create(int port, rr_state_c *state);

    /**
     * @fn despose
     * @brief stops the server
     */
    void dispose();

   /**
    * @fn recieve
    * @brief recieves inbound messages (in its own thread)
    * @param socket connection to environment
    * @return returns vector containing found operations. Each operation MUST end with 
    * 0x1E ASCII character, defined as end-of-record.
    */
    static vector<json> recieve(socket_env *senv, char *buffer);

    /**
     * @fn sendto
     * @brief  outbound messages in its own thread.
     * @param socket connection to environment
     */
    static size_t sendto(socket_env *senv, json msg);

    /** 
     * @fn accept
     * @brief ready to accept conenctions.
     */
    pthread_t rr_accept();

   private:
    int _sockfd = 0;
    int _max_connections = 5;
    socket_env *_sockenv = new socket_env();

    static void *accept_conn(void *senv);
    static void *recieve_actions(void *in_senv);
};
}  // namespace rrobot

#endif