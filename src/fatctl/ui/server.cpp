/**
 * @brief concret implementation of thre server.
 *
 * The flow for this service is:
 *
 * 1. recieve request
 * 2. Upgrade connection to websocket (if conditions are met)
 * 3. Communicate with the UI.
 */

#include "server.hpp"

using namespace std;
using namespace rrobot;
using json = nlohmann::json;

/**
 * Receives messages from the UDP/IP socket actions.
 *
 */
void *fatcnt_server::recieve(void *in) {
    socket_env *senv = static_cast<socket_env*>(in);
    char *buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char))); 
    
    memset(buffer, 0, BUFSIZ);
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    size_t n = recv(senv->get_socket(), buffer, BUFSIZ * sizeof(char), 0);
    string s = buffer;

    // put the request into a queue so that it can be processed by something 
    // else.
    if (!json::accept(s)) {
        // TODO: note that the requst was invalid and ignore
    }
    json j = json::parse(s);
    // TODO: perform some validation here.
    json *result; 
    memcpy(result, &j, j.size());

    return static_cast<void *>(result);
}

/*
 * Ideally read from a queue and perform required actions.
 */
void *fatcnt_server::send(void *in) {
    socket_env *senv = static_cast<socket_env*>(in);
    size_t n = 0; //sendto(senv->get_sockfd(), );
    return reinterpret_cast<void *>(n);
}

socket_env* fatcnt_server::create(int port, rr_state_c *state) {
    socket_env *sockenv = new socket_env();
    try {
        struct sockaddr_in servaddr;

        // Creating socket file descriptor
        if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw RR_WS_UNABLE_TO_CONNECT;
        }
        memset(&servaddr, 0, sizeof(servaddr));

        // Filling server information
        servaddr.sin_family = AF_INET;  // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        // Bind the socket with the server address
        if (bind(_sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            throw RR_WS_UNABLE_TO_CONNECT;
        }

        
        sockenv->set_sockfd(_sockfd);
        sockenv->set_servaddr(&servaddr);
        sockenv->set_state(state);

        listen(_sockfd, _max_connections);
        
        state->get_observers();

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        // return 1;
    }
    return sockenv;
}

void fatcnt_server::despose() { close(_sockfd); }
