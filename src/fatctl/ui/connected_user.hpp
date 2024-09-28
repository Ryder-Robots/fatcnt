#ifndef CONNECTED_USER_HPP
#define CONNECTED_USER_HPP

#include <string>
#include <microhttpd.h>
#include <microhttpd_ws.h>

using namespace std;
namespace rrobot {
class connected_user {
   public:
    connected_user(MHD_socket fd, MHD_UpgradeResponseHandle* urh, string extra_in):
        _fd(fd),
        _urh(urh),
        _extra_in(extra_in)
    {}

    MHD_socket get_fd() {
        return _fd;
    }

    struct MHD_UpgradeResponseHandle* get_urh() {
        return _urh;
    }

     struct MHD_WebSocketStream* get_ws() {
        return _ws;
     }
     
   private:
    /* the TCP/IP socket for reading/writing */
    MHD_socket                        _fd;
    /* the UpgradeResponseHandle of libmicrohttpd (needed for closing the socket) */
    struct MHD_UpgradeResponseHandle* _urh;
    
    /* the websocket encode/decode stream */
    struct MHD_WebSocketStream*       _ws;

    /* the possibly read data at the start (only used once) */
    string                            _extra_in;

    struct timespec                   _ping_start;
    /* the message used for the ping (must match the pong response)*/
    string                            _ping_message;
};
}  // namespace rrobot

#endif  // CONNECTED_USER_HPP