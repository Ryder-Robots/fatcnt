#include "fatctl.h"

using namespace rrobot;

int main() {

    //TODO: this should all come from an uploaded config
    // Create webserver
    fatcnt_server fs = fatcnt_server(
         httpserver::webserver(httpserver::create_webserver(8081)
         .start_method(httpserver::http::http_utils::INTERNAL_SELECT)
         .max_threads(5))
    );

    // create resource
    rr_connection connection = rr_connection();
    fs.register_resource(&connection, "/connection");

    // start server
    fs.create();

    return 0;
}