#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <dlib/logger.h>

#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/stateiface.hpp>
#include "external.hpp"

namespace rrobot {
    class RrServer : public External {
        public:
            void init(Environment* environment, StateIface* state) override;

            ssize_t recv_rr(void* buffer, size_t bufsz) override;

            int accept_rr() override;

            void close_rr() override;

            ssize_t send_rr(const void *buf, size_t bufsz)  override;

            ssize_t available() override;

            int shutdown_rr() override;

        private:
            int                 _socket = 0;
            int                 _sockfd = 0;
            struct sockaddr_in *_servaddr = nullptr;
    };
}

#endif // SERVER_HPP