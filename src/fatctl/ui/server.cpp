/**
 * @brief concret implementation of thre server.
 */

#include "server.hpp"

using namespace rrobot;

void fatcnt_server::create() {
    try {
        _ws.start(true);
    } catch (const std::exception& e) {
        //TODO: throw exception this is fatal.
        std::cout << "Exception " << e.what() << "\n";
    }
}

void fatcnt_server::register_resource(httpserver::http_resource* resource, std::string path) {
    _ws.register_resource(path, resource, true);
}