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
    fatcnt_server() {}

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

};
}  // namespace rrobot

#endif