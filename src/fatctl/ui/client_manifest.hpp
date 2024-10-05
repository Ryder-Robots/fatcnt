/**
 * 
 */

#ifndef CLIENT_MANIFEST_HPP
#define CLIENT_MANIFEST_HPP

#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
/**
 * @brief returned by client and used to indicate how the drone should communicate with
 * the UX client.
 */
namespace rrobot {
    class client_manifest {
    public:
        client_manifest(
            string client_id,
            string swname,
            string sw_version
        ):
            _client_id(client_id),
            _swname(swname),
            _swversion(sw_version)
        {}

        /**
         * @fn get_client_id
         * @brief returns the client id
         */
        string get_client_id() {
            return _client_id;
        }

        /**
         * @fn get_sw_name
         * @brief returns the software name of the UI.
         */
        string get_sw_name() {
            return _swname;
        }

        /**
         * @fn get_swversion
         * @brief returns the software version of the UI.
         */
        string get_swversion() {
            return _swversion; 
        }

    private:
        const string _client_id;
        const string _swname;
        const string _swversion;
    };
}

#endif // CLIENT_MANIFEST_HPP