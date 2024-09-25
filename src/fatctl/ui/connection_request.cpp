/*
 * include legal agreement.
 */

#include "fatctl/ui/connection_request.hpp"

using namespace rrobot;

/**
 * @brief concrete implementation of connection request.
 */

connection_request::connection_request(string request) :
    _at_hash(""),
    _client_id(""),
    _swname(""),
    _swversion(""),
    _hwmodel("")
{
    if (!json::accept(request)) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }

    try {
        const json js = json::parse(request);
        // check for required fields first.
        for (auto k : {KEY_client_id, KEY_at_hash}) {
            if (!js.contains(k)) {
                // missing required field
                throw RR_WS_STATUS_CANNOT_ACCEPT;
            }
        }

        js[KEY_client_id].get_to(_client_id);
        js[KEY_at_hash].get_to(_at_hash);

        if (js.contains(KEY_swname)) {
            js[KEY_swname].get_to(_swname);
        }

        if (js.contains(KEY_swversion)) {
            js[KEY_swversion].get_to(_swversion);
        }

        if (js.contains(KEY_hwmodel)) {
            js[KEY_hwmodel].get_to(_hwmodel);
        }
    } catch (...) {
        throw RR_WS_STATUS_CANNOT_ACCEPT;
    }
}


 const string connection_request::get_client_id() {
    return _client_id;
 }

const string connection_request::get_at_hash() {
    return _at_hash;
}

const string connection_request::get_swname() {
    return _swname;
}

const string connection_request::get_swversion() {
    return _swversion;
}

const json connection_request::get_hwmodel() {
    return _hwmodel;
}