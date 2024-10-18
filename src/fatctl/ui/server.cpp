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

/**
 * read ingress and encode it.
 */
json fatcnt_server::recieve(socket_env *senv, char *buffer) {
    memset(buffer, 0, BUFSIZ);
    size_t n = recv(senv->get_socket(), buffer, BUFSIZ * sizeof(char), 0);
    string s = buffer;

    // put the request into a queue so that it can be processed by something
    // else.
    if (!json::accept(s)) {
        throw RR_INVALID_FMT;
    }
    json j = json::parse(s);

    return j;
}

// encode each action recieved from UI to something that can be used in
// wiringPi.
//
// this thread will need to a pthread_cond_t to alert processing to do something with the 
// request.
void *fatcnt_server::recieve_actions(void *in_senv) {
    
    // Control the flow of inbound requests.
    socket_env *senv = reinterpret_cast<socket_env *>(in_senv);
    pthread_mutex_t lock = senv->get_read_lock();
    pthread_mutex_lock(&lock);

    try {
        char *buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char)));
        size_t available = 0;
        int err = 0;

        while(!senv->is_exit()) {
            available = 0;
            if ((err = ioctl(senv->get_socket(), FIONREAD, &available)) == 0) {
                if (available) {
                    try {
                        json action = recieve(senv, buffer);
                        dlog_ui << dlib::LDEBUG << "received action: adding to queue";
                        senv->get_state()->put_event(action);
                    } catch (json::parse_error& ex) {
                        dlog_ui << dlib::LERROR << "recieved exception, going to handle it.";
                    }
                }
            } else if (err == -1) {
                dlog_ui << dlib::LERROR << "network error has occurred";
            }
        }
    } catch (...) {
        dlog_ui << dlib::LERROR << "unhandled exception has occurred";
    }
    pthread_mutex_unlock(&lock);

    return static_cast<int *>(0);
}


/*
 * Ideally read from a queue and perform required actions.
 */
size_t fatcnt_server::sendto(socket_env *senv, json msg) {
    return send(senv->get_socket(), msg.dump().c_str(), msg.dump().size() * sizeof(char), 0);
}

/**
 * create an interface to a bound port that the drone can accept requests and send
 * to a UI.
 */
socket_env *fatcnt_server::create(int port, rr_state_c *state) {
    try {
        // create read lock
        pthread_mutex_t read_lock = _sockenv->get_read_lock();
        if (pthread_mutex_init(&read_lock, NULL) != 0) {
            throw RR_MUTEX_ERR;
        }

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
            pthread_t pt;
            if (0 != pthread_create(&pt, NULL, &recieve_actions, senv))
                throw RR_UNABLE_TO_CREATE_ACTION_THREAD;
            
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