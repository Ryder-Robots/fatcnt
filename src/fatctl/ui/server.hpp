/*!
 * defines the server.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <stdexcept>
#include <microhttpd.h>
#include <microhttpd_ws.h>
#include <string.h>
#include <stdint.h>

#include "fatctl/environment/state.hpp"

#define PAGE_NOT_FOUND "404 Not Found"
#define MHD_OPTS MHD_ALLOW_UPGRADE | MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG
#define PORT 8081
#define TIMEOUT (uint32_t) 120

namespace rrobot {
    class fatcnt_server {
    public:
        fatcnt_server():
            _ws(NULL){}

        void create();
        void close();

    private:
        struct MHD_Daemon *_ws;
    };
}

#endif