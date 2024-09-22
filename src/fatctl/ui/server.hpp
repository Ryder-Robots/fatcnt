/*!
 * defines the server.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <httpserver.hpp>
#include <stdexcept>

namespace rrobot {
    class fatcnt_server {
    public:
        #define DEFAULT_THREADS 5
        #define DEFAULT_PORT 8080
        #define DEFAULT_THREAD_METHOD httpserver::http::http_utils::INTERNAL_SELECT

        /** 
         * @fn fatcnt_server
         * @brief contructor
         * @param threads threads to use
         * @param port port to listen to
         */
        fatcnt_server(
            httpserver::webserver ws,
            const int threads = DEFAULT_THREADS, 
            const int port = DEFAULT_PORT, 
            const httpserver::http::http_utils::start_method_T thread_method = DEFAULT_THREAD_METHOD
        ):
            _threads(threads),
            _port(port),
            _thread_method(thread_method),
            _ws(ws)
        {
           
        }

        /**
         * @fn create
         * 
         * @brief creates and starts a new instance of Fat Conotroller UI.
         */
        void create();

        /**
         * @fn register_resource
         * 
         * @brief  a resource.
         * 
         * @param resource handler to execute the resousce action.
         * @param path that the resource is attached to.
         */
        void register_resource(httpserver::http_resource* resource, std::string path);

        private:
            int _threads;
            int _port;
            httpserver::http::http_utils::start_method_T _thread_method;
            httpserver::webserver _ws;
    };
}

#endif