/**
 * @brief connection resource file
 */

#include "connection.hpp"

using namespace  rrobot;

/**
 * @brief concrete implentation of render. This should return back the bearer token, so that all are 
 * transactions are secure.
 */
std::shared_ptr<httpserver::http_response>  rr_connection::render(const httpserver::http_request&) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello World!!!", 200));
}