#ifndef SOCKET_ENV_HPP
#define SOCKET_ENV_HPP

#include  <netinet/in.h>
#include <nlohmann/json.hpp>
#include "fatctl/environment/state.hpp"

using json = nlohmann::json;

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

        /**
         * @fn set_state
         * @brief hardware environment state.
         * @param class that displays the current hardware state.
         */
        // When setting the state use a mutex to control that it can be set.
        void set_state(rr_state_c *state) {
            _state = state;
        }

        /**
         * @fn get_state
         * @brief returns current environment state
         */
        // Note that mutexes SHOULD be used to verify that the state variable can
        // be read.
        rr_state_c *get_state() {
            return _state;
        }

    protected:
        /**
         * @fn put_send_event
         * @param JSON event to send.
         */
        /*
         * at this stage plave event on a  buffer so it can be used later.
         */
        void put_event(json *event) {
            _event = event;
        }

        json*  get_event() {
            return _event;
        }



    private:
        int                 _sockfd;
        struct sockaddr_in *_servaddr;
        rr_state_c         *_state;
        json               *_event;
};
}

#endif