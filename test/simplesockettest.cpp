#include <err.h>
#include <sys/socket.h>
#include "fatctl/ui/socket_env.hpp"
#include "fatctl/ui/server.hpp"

using namespace rrobot;

int main() {
    rr_state_c *state = new rr_state_c();
    fatcnt_server server = fatcnt_server();

    socket_env *senv = server.create(8081, state);
    int clientSocket = accept(senv->get_sockfd(), nullptr, nullptr);
    if (clientSocket == -1) {
        std::cout << "log(-1) failed: " << std::strerror(errno) << '\n';
    }
    senv->set_socket(clientSocket);

    // create threads at this point.
    char *buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char)));
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    server.recieve(senv, buffer);
    free(buffer);
    server.dispose();
    
    return 0;
}