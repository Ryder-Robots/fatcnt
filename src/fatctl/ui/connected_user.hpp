/*
 * LICENCE: MIT licence (see LINCENCE.txt for full details)
 */

#ifndef CONNECTED_USER_HPP
#define CONNECTED_USER_HPP

#include <microhttpd.h>
#include <microhttpd_ws.h>

#include <string>

/**
 * @brief stores websocket options for a single user.
 */
using namespace std;
namespace rrobot {
class connected_user {
   public:

    /** 
     * @fn connected_user
     * @brief class constructor.
     * @param fd socket file descriptor
     * @param urh upgrade response handle (see https://www.rfc-editor.org/rfc/rfc6455.html section 1.3)
     * @param extra_in  (reseved use)
     */
    connected_user(MHD_socket fd, MHD_UpgradeResponseHandle* urh, string extra_in)
        : _fd(fd), _urh(urh), _extra_in(extra_in) {}

    /** 
     *  @fn ~connected_user
     *  @brief class descontructor
     */
    ~connected_user() {
        MHD_websocket_stream_free(_ws);
    }

    /**
     * @fn get_fd
     * @brief returns socket file descriptor
     */
    MHD_socket get_fd() { return _fd; }

    /**
     * @fn
     * @brief returns Upgrade Request Handle (URH)
     */
    struct MHD_UpgradeResponseHandle* get_urh() { return _urh; }

    /**
     * @fn get_ws
     * @brief IO Stream that handles encoded and decoding of websocket data.
     * @return websocket stream
     */
    struct MHD_WebSocketStream* get_ws() { return _ws; }

    /**
     * @fn set_ws
     * @brief sets websocket stream
     */
    void set_ws(struct MHD_WebSocketStream* ws) { _ws = ws; }

   private:
    /* the TCP/IP socket for reading/writing */
    MHD_socket _fd;
    /* the UpgradeResponseHandle of libmicrohttpd (needed for closing the socket) */
    struct MHD_UpgradeResponseHandle* _urh;

    /* the websocket encode/decode stream */
    struct MHD_WebSocketStream* _ws;

    /* the possibly read data at the start (only used once) */
    string _extra_in;

    struct timespec _ping_start;
    /* the message used for the ping (must match the pong response)*/
    string _ping_message;
};
}  // namespace rrobot

#endif  // CONNECTED_USER_HPP