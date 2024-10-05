#ifndef CLIENT_MANIFEST_HPP
#define CLIENT_MANIFEST_HPP

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "fatctl/ui/websocket_status_code.hpp"

using namespace std;
using json = nlohmann::json;

namespace rrobot {

#define KEY_at_hash "at_hash"
#define KEY_client_id "client_id"
#define KEY_swname "swname"
#define KEY_swversion "swversion"
#define KEY_hwmodel "hwmodel"

/**
 * @brief expected request from the client. This occurs after websokcet handshake has been performed,
 * and is the first action sent by the UI,  if this is not sent then the connection is terminated,
 * and the drone refuses aby communication.
 *
 * all attributes should comply with https://www.iana.org/assignments/jwt/jwt.xhtml
 */
class client_manifest {
   public:
    /**
     * @fn connection_request
     * @brief class constructor
     * @param req inbound json string describing request
     */
    client_manifest(const string req);

    /**
     * @fn get_at_hash
     * @brief access troken provided by user. Used to verify the account.
     */
    const string get_at_hash();

    /**
     * @fn get_client_id
     * @brief returns the client id
     */
    const string get_client_id();

    /**
     * @fn get_swname
     * @brief name of the UI client.
     */
    const string get_swname();

    /**
     * @fn get_swversion
     * @brief UI client version
     */
    const string get_swversion();

    /**
     *  @fn get_hwmodel
     *  @brief concerned with the monitor here, attributes are:
     *    pwidth  : width in pixals
     *    pheight : height in pixals
     */
    const json get_hwmodel();

   private:
    // Access Token hash value
    string _at_hash;

    // Client Identifier
    string _client_id;

    // The name of the software running in the entity
    string _swname;

    // The version of software running in the entity
    string _swversion;

    // Model identifier for hardware
    json _hwmodel;
};
}  // namespace rrobot
#endif  // CONNECTION_REQUEST_HPP