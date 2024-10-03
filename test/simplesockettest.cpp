#include "fatctl/ui/socket_env.hpp"
#include "fatctl/ui/server.hpp"

using namespace rrobot;

int main() {
    rr_state_c *state = new rr_state_c();
    fatcnt_server server = fatcnt_server();

    socket_env *senv = server.create(8081, state);
    server.recieve(senv);

    server.despose();
    
    return 0;
}