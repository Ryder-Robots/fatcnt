/*!
 * defines the server.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h> 
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "fatctl/environment/state.hpp"
#include "fatctl/ui/websocket_status_code.hpp"
#include "fatctl/ui/socket_env.hpp"


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
    void despose();

   /**
    * @fn recieve
    * @brief recieves inbound messages (in its own thread)
    * @param socket connection to environment
    */
    static void *recieve(void *senv);

    /**
     * @fn send
     * @brief  outbound messages in its own thread.
     * @param socket connection to environment
     */
    static void *send(void *senv);

   private:
    int _sockfd;
    int _max_connections = 5;
};
}  // namespace rrobot

#endif