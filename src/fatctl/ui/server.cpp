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
    uint8_t *buffer = static_cast<uint8_t *>(malloc(PTRDIFF_MAX)); 
    
    bzero(buffer, PTRDIFF_MAX);
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    socklen_t len;
    int n = recvfrom(senv->get_sockfd(), buffer, PTRDIFF_MAX, MSG_WAITALL, (struct sockaddr *)senv->get_servaddr(), &len);

    string s;
    for (int i = 0;i < n; i++) {
        s += buffer[i];
    }

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

void *fatcnt_server::send(void *senv) {
    return static_cast<void *>(0);
}

void fatcnt_server::create(int port) {
    try {
        struct sockaddr_in servaddr;

        // Creating socket file descriptor
        if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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

        rrobot::rr_state_c *RR_STATE;
        socket_env *sockenv = new socket_env();
        sockenv->set_sockfd(_sockfd);
        sockenv->set_servaddr(&servaddr);
        sockenv->set_state(RR_STATE);

        
        RR_STATE->get_observers();

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        // return 1;
    }
}

void fatcnt_server::despose() { close(_sockfd); }
