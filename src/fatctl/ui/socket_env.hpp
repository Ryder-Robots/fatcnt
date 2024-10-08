#ifndef SOCKET_ENV_HPP
#define SOCKET_ENV_HPP

#include <netinet/in.h>

#include <nlohmann/json.hpp>

#include "fatctl/environment/state.hpp"
#include "fatctl/ui/ux_manifest.hpp"

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
    void set_sockfd(int sockfd) { _sockfd = sockfd; }

    /**
     * @fn get_sockfd
     * @brief gets the socket file descriptor
     */
    int get_sockfd() { return _sockfd; }

    void set_socket(int socket) { _socket = socket; }

    int get_socket() { return _socket; }

    /**
     * @fn set_serveraddr
     * @brief sets server address.
     */
    void set_servaddr(struct sockaddr_in *servaddr) { _servaddr = servaddr; }

    /**
     * @fn get_servaddr
     * @brief gets server address
     * @return server address
     */
    struct sockaddr_in *get_servaddr() { return _servaddr; }

    /**
     * @fn set_state
     * @brief hardware environment state.
     * @param class that displays the current hardware state.
     */
    // When setting the state use a mutex to control that it can be set.
    void set_state(rr_state_c *state) { _state = state; }

    /**
     * @fn get_state
     * @brief returns current environment state
     */
    // Note that mutexes SHOULD be used to verify that the state variable can
    // be read.
    rr_state_c *get_state() { return _state; }

    /**
     * @fn put_send_event
     * @param JSON event to send.
     */
    /*
     * at this stage plave event on a  buffer so it can be used later.
     */
    void put_event(json *event) { _event = event; }

    /**
     * @fn get_event
     * @brief return last event from event queue
     * @return last event as JSON
     */
    json *get_event() { return _event; }

    /**
     * @fn set_is_exit
     * @brief set exit status to true.
     */
    void set_is_exist(bool is_exit) { _is_exit = is_exit; }

    /**
     * @fn is_exit
     * @brief returns exit status
     * @return when true classes should call dispose or deconstructor.
     */
    bool is_exit() { return _is_exit; }

    void set_ux_manifest(ux_manifest *ux) { _ux_manifest = ux; }

    ux_manifest *get_ux_manifest() { return _ux_manifest; }

   private:
    int _sockfd;

    // an accepted socket by client.
    int _socket;
    struct sockaddr_in *_servaddr;
    rr_state_c *_state;
    json *_event;
    bool _is_exit = false;
    ux_manifest *_ux_manifest;
};
}  // namespace rrobot

#endif