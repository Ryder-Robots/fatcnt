/*
 * TODO: add licence
 */

#include <string>
#include <microhttpd.h>
#include <microhttpd_ws.h>
#include <nlohmann/json.hpp>

#define ALG_HMAC   "HMAC"
#define ALG_SHA256 "SHA256"
#define ALG_RSA    "RSA"

#define KEY_ALG "alg"
#define KEY_TYP "typ"

using json = nlohmann::json;

/*
 * @brief 
 *  
 * Defines JWT format sent to UI to allow comminication with socket. Used for
 * authentication, and authorization.
 * 
 * reference: https://jwt.io/introduction/
 * reference: https://www.iana.org/assignments/jwt/jwt.xhtml
 * https://json.nlohmann.me/api/basic_json/contains/
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
            _typ(typ)
        {
            construct(alg, typ);
        }

        /**
         * @fn class contructor
         * @brief creates connection object based from an input string.
         */
        connection_header(string s) {
            if (!json::accept(s)) {
                throw MHD_WEBSOCKET_STATUS_PARAMETER_ERROR;
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

    
    private:
        void construct(string alg, string typ);
        void construct(string json);

        string _alg;
        string _typ;
    };
}