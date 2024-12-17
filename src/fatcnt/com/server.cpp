#include "server.hpp"

using namespace rrobot;


dlib::logger dlog_svr("rr_robot_svr");

void RrServer::init(Environment* environment, StateIface* state) {
    dlog_svr.set_level(dlib::LALL);
    struct sockaddr_in servaddr;
    Environment env = *environment;
    // Creating socket file descriptor
    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        dlog_svr << dlib::LERROR << "unable to create connection for UI " << strerror(errno);
        throw BadConnectionException("unable to create connection for UI " + string(strerror(errno)));
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(env.getServer().getPort());

    if (bind(_sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        dlog_svr << dlib::LFATAL << "fatal error occured: " << strerror(errno);
        throw BadConnectionException("could not bind to port " + string(strerror(errno)));
    }
    listen(_sockfd, env.getServer().getMaxConnections());
    dlog_svr << dlib::LINFO << "listening for connections on port " << env.getServer().getPort();
}

ssize_t RrServer::recv_rr(void* buffer, size_t bufsz) {
    size_t n = recv(_socket, buffer, bufsz, 0);
    return n;
}


int RrServer::accept_rr() {
    _socket = accept(_sockfd, nullptr, nullptr);
    return _socket;
}

void RrServer::close_rr() {
    close(_sockfd);
}


ssize_t RrServer::send_rr(const void *buf, size_t bufsz) {
    return send(_socket, buf, bufsz, 0);
}

size_t RrServer::available() {
    size_t available = 0;
    return ioctl(_socket, FIONREAD, &available);
}