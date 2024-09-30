#ifndef SOCKET_ENV_HPP
#define SOCKET_ENV_HPP

#include  <netinet/in.h>
#include "fatctl/environment/state.hpp"

/**
 * @brief controls communication to UX
 */
namespace rrobot {
class socket_env {
    public:

        /**
         * @fn set_sockfd
         * @brief sets socket file descriptor
         * @param socket file descriptor
         */
        void set_sockfd(int sockfd) {
            _sockfd = sockfd;
        }

        /**
         * @fn get_sockfd
         * @brief gets the socket file descriptor
         */
        int get_sockfd() {
            return _sockfd;
        }

        /**
         * @fn set_serveraddr
         * @brief sets server address.
         */
        void set_servaddr(struct sockaddr_in *servaddr) {
            _servaddr = servaddr;
        }

        /** 
         * @fn get_servaddr
         * @brief gets server address
         * @return server address
         */
        struct sockaddr_in *get_servaddr() {
            return _servaddr;
        }

        void set_state(rr_state_c *state) {
            _state = state;
        }

        rr_state_c *get_state() {
            return _state;
        }

    private:
        int _sockfd;
        struct sockaddr_in *_servaddr;
        rr_state_c *_state;
};
}

#endif