/*
 * TODO: add licence

 *
 * For authentocation I think what happens is that drone first sends a secret to the client, it is up to the client 
 * to send back the JWT that includs that secret, from the public key which only the server should be able to decrypt.
 */

#include "connection.hpp"

using namespace rrobot;


/*
 * deserialize an inbound JSON header object.
 */
void connection_header::construct(string s) {
    if (!json::accept(s)) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    json j = json::parse(s);
    connection_header(j[KEY_ALG], j[KEY_TYP]);
}

json connection_header::to_json() {
    json j;
    return j;
}

