/*
 * TODO: add licence

 *
 * For authentocation I think what happens is that drone first sends a secret to the client, it is up to the client 
 * to send back the JWT that includs that secret, from the public key which only the server should be able to decrypt.
 */

#include "connection.hpp"

using namespace rrobot;

/*
 * class contructor
 */
void connection_header::construct(string alg, string typ) {
    bool found = false;
    for (const auto& [key, value] : _sup_algo) {
        if (key == alg) {
            found = true;
            _evp_md = value;
            break;
        }
    }

    if (!found) {
        // algorithm not implemented.
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    // JWT is the only supported protocol.
    if ("JWT" != typ) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    _alg = alg;
    _typ = typ;
}

/*
 * deserialize an inbound JSON header object.
 */
void connection_header::construct(string s) {
    if (!json::accept(s)) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    json j = json::parse(s);

    if (!(j.contains(KEY_ALG) && j.contains(KEY_TYP))) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    connection_header(j[KEY_ALG], j[KEY_TYP]);
}

json connection_header::to_json() {
    json j;
    j[KEY_ALG] = _alg;
    j[KEY_TYP] = _typ;
    return j;
}

string connection_header::encode() {
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen;

    string json = to_json();

    //TODO: This needs to be the PEM secret given by client
    string secret = "";
    
    HMAC(EVP_sha256(), secret.c_str(), static_cast<int>(secret.size()),
         reinterpret_cast<unsigned char const*>(json.data()), static_cast<int>(json.size()), hash.data(), &hashLen);

    return std::string{reinterpret_cast<char const*>(hash.data()), hashLen};
}
