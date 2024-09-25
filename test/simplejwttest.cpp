#include <iostream>

#include "fatctl/ui/connection_request.hpp"

using namespace std;
using namespace rrobot;

int main() {
    string request = R"({"at_hash": "a93fdd95612965ede0c1deae52ac85ec", "client_id":"00a1"})";
    cout << "request = " << request << "\n";
    connection_request hdr = connection_request(request);

    assert("00a1" == hdr.get_client_id());
    assert("a93fdd95612965ede0c1deae52ac85ec" == hdr.get_at_hash());

    request = R"({"at_hash": "a93fdd95612965ede0c1deae52ac85ec", "client_id":"00a1", "swname":"rr-client", "swversion":"1.0.0", "hwmodel":{
        "pwidth": 4480,
        "pheight": 2520
    }})";
    cout << "request = " << request << "\n";
    hdr = connection_request(request);

    assert("rr-client" == hdr.get_swname());
    assert("1.0.0" == hdr.get_swversion());

    return 0;
}