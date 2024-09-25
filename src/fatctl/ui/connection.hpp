/*
 * TODO: add licence
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <array>
#include <map>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include "fatctl/ui/websocket_status_code.hpp"


using json = nlohmann::json;

/*
 * @brief 
 *  
 * Once connection has been established this response is sent to the UI client, it should give the client
 * enough information in order to operate the drone given their level of entitlements.
 * 
 * references: 
 * https://jwt.io/introduction/
 * https://www.iana.org/assignments/jwt/jwt.xhtml
 * https://json.nlohmann.me/api/basic_json/contains/
 * https://www.rfc-editor.org/rfc/rfc
 */
using namespace std;
namespace rrobot {
    class connection_reponse {
    public:
        /** 
         * @fn connection_header
         * @brief class constructor
         * @param alg algoritm to use for encoding and decoding JWT
         * @param typ type of authorization header. 
         */
        connection_header(string alg = "HS256", string typ = "JWT"):
            _alg(alg),
            _typ(typ),
            _evp_md(EVP_sha256)
        {
            construct(alg, typ);
        }

        /**
         * @fn class contructor
         * @brief creates connection object based from an input string.
         */
        connection_header(string s) {
            if (!json::accept(s)) {
                throw RR_WS_STATUS_CANNOT_ACCEPT;
            }

            json j = json::parse(s);

        }

        /**
         * @fn encode
         * @brief encode JWT header
         */
        string encode();

        /** 
         * @fn hrcmp
         * @brief compare that inbound contents are the same as this header.
         * @param connection_header to compare to
         * @return if comparison is the same then return '0'
         */
        int hdrcmp(const connection_header &in);

        /**
         * @fn to_json
         * @brief return JSON representation.
         * @return json representation
         */
        json to_json();

    
    private:
        void construct(string json);

        // Value used to associate a Client session with an ID Token (MAY also be 
        // used for nonce values in other applications of JWTs)
        const string nonce;

        // Time when the authentication occurred
        const time_t auth_time;

        // Access Token hash value, this key can will be regenerated and MUST be stored client side, and used for the 
        // the next authentication attempt. 
        const string at_hash;

        // Client Identifier
        const string client_id;

        // The name of the software running in the entity
        const string swname;

        // The version of software running in the entity
        const string swversion;

        // Model identifier for hardware
        const string hwmodel;

        // Hardware Version Identifier
        const string hwversion;

        // Manifests describing the software installed on the entity
        // also describes drone capabilities, such as gyroscope, acelometer etc.
        const json manifests;

        // Measurements of the drone, 
        const json measurements;

        // Geohash String or Array
        const json geohash;

        // Entitlements, what the client can do while running the drone.
        const json entitlements;
    };
}
#endif