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

#define KEY_ALG "alg"
#define KEY_TYP "typ"

using json = nlohmann::json;

/*
 * @brief 
 *  
 * Defines JWT format sent to UI to allow comminication with socket. Used for
 * authentication, and authorization.
 * 
 * references: 
 * https://jwt.io/introduction/
 * https://www.iana.org/assignments/jwt/jwt.xhtml
 * https://json.nlohmann.me/api/basic_json/contains/
 * https://www.rfc-editor.org/rfc/rfc
 */
using namespace std;
namespace rrobot {
    class connection_header {
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
        void construct(string alg, string typ);
        void construct(string json);

        string _alg;
        string _typ;
        const EVP_MD* (*_evp_md) (void);
        const map<string,  const EVP_MD *(*)()> _sup_algo{
            {"HS256", EVP_sha256}
        };
    };
}
#endif