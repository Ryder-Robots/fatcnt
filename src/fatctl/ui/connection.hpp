/*!
 * authentication connection.
 *
 * This should use the path /connection to get the connection.
 */

#ifndef RR_CONNECTION_HPP
#define RR_CONNECTION_HPP

#include <iostream>
#include <httpserver.hpp>

namespace rrobot {
    class rr_connection : public httpserver::http_resource  {
    public:
        rr_connection() {}

        std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&);
    
    private:
        std::string data;
    };
}
#endif