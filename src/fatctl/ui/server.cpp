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
dlib::logger dlog_ui("rr_robot_ui");
json fatcnt_server::recieve(socket_env *senv, char *buffer) {
    memset(buffer, 0, BUFSIZ);
    size_t n = recv(senv->get_socket(), buffer, BUFSIZ * sizeof(char), 0);
    string s = buffer;

    // put the request into a queue so that it can be processed by something
    // else.
    if (!json::accept(s)) {
        // TODO: note that the requst was invalid and ignore
    }
    json j = json::parse(s);

    return j;
}

/*
 * Ideally read from a queue and perform required actions.
 */
size_t fatcnt_server::sendto(socket_env *senv, json msg) {
    return send(senv->get_socket(), msg.dump().c_str(), msg.dump().size() * sizeof(char), 0);
}

socket_env *fatcnt_server::create(int port, rr_state_c *state) {
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
            dlog_ui << dlib::LFATAL << "fatal error occured: " << strerror(errno);

            throw RR_WS_UNABLE_TO_CONNECT;
        }

        _sockenv->set_sockfd(_sockfd);
        _sockenv->set_servaddr(&servaddr);
        _sockenv->set_state(state);

        listen(_sockfd, _max_connections);
     } catch (const std::exception &ex) {
        if (_sockfd != -1) {
            dispose();
        }
        dlog_ui << dlib::LFATAL << "fatal error occured: " << ex.what();
        return _sockenv;
    }
    return _sockenv;
}

void fatcnt_server::dispose() { close(_sockfd); }

// Create the accept thread.
pthread_t fatcnt_server::rr_accept() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, &accept_conn, _sockenv);
    return ptid;
}

void *fatcnt_server::accept_conn(void *in) {
    socket_env *senv = static_cast<socket_env *>(in);
    
    while (!senv->is_exit()) {
        try {
            int clientSocket = accept(senv->get_sockfd(), nullptr, nullptr);
            senv->set_socket(clientSocket);
            dlog_ui << dlib::LINFO << "recieved connection";
            char *buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char)));
            
            struct sockaddr_in cliaddr;
            memset(&cliaddr, 0, sizeof(cliaddr));
            json manifest = recieve(senv, buffer);
            dlog_ui << dlib::LINFO << "authorization successful";
            const std::string request = reinterpret_cast<const char*>(buffer);
            ux_manifest *ui_manifest = new ux_manifest(request);

            // TODO: authorize here

            senv->set_ux_manifest(ui_manifest);
            free(buffer);

            sendto(senv, senv->get_state()->get_manifest());

            // create read and write threads, these threads will interact with the environment,
            // NN, and Renforcement Learning algoritm until program termination.
        } catch (const std::exception &ex) {
            dlog_ui << dlib::LFATAL << "fatal error occured: " << ex.what();
        } catch (int e) {
            switch(e) {
                case RR_WS_STATUS_CANNOT_ACCEPT:
                    dlog_ui << dlib::LFATAL << "invalid request was recieved";
            }
        } catch (...) {
            dlog_ui << dlib::LFATAL << "unhandled exception has occured: "; 
        }
    }
    return static_cast<void *>(0);
}