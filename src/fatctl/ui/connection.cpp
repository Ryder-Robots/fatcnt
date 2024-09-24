/*
 * TODO: add licence
 */

#include "connection.hpp"

using namespace rrobot;

/*
 * class contructor
 */
void connection_header::construct(string alg, string typ) {
    bool found = false;
    for (auto a : {ALG_HMAC, ALG_SHA256, ALG_RSA}) {
        if (a == alg) {
            found = true;
            break;
        }
    }

    if (!found) {
        // algorithm not implemented.
        throw MHD_WEBSOCKET_STATUS_PROTOCOL_ERROR;
    }

    // JWT is the only supported protocol.
    if ("JWT" == typ) {
        throw MHD_WEBSOCKET_STATUS_PROTOCOL_ERROR;
    }

    _alg = alg;
    _typ = typ;
}

/*
 * deserialize an inbound JSON header object.
 */
void connection_header::construct(string s) {
    if (!json::accept(s)) {
        throw MHD_WEBSOCKET_STATUS_PARAMETER_ERROR;
    }

    json j = json::parse(s);

    if (!(j.contains(KEY_ALG) && j.contains(KEY_TYP))) {
        throw MHD_WEBSOCKET_STATUS_PARAMETER_ERROR;
    }

    connection_header(j[KEY_ALG], j[KEY_TYP]);
}