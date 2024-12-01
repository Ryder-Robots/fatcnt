#include <fatcnt/protocols/common/curators/rrp/msp_ident_curator.hpp>

using namespace rrobot;

/**
 * @fn deserialize
 * @brief 
 * deserialize inbound json.
 */
Event* msp_ident_curator::deserialize(json in) {
    msp_ident* payload = new msp_ident();
    Event* event = new Event(MSP_IDENT, _direction, payload);
    validate("version", in);
    payload->set_version(in["version"]);

    // MULTITYPE_T
    validate("multitype", in);
    VALID_MULTITYPE_KEYS_INIT;
    if (VALID_MULTITYPE_KEYS.find(in["multitype"]) == VALID_MULTITYPE_KEYS.end()) {
        throw UnsupportedAttribute(in["multitype"].get<string>() + " is not an MULTITYPE");
    }
    payload->set_multitype(VALID_MULTITYPE_KEYS.at(in["multitype"]));

   
    //MSP_VERSION
    validate("msp_version", in);
    VALID_MSP_VERSION_KEYS_INIT;
    if (VALID_MSP_VERSION_KEYS.find(in["msp_version"]) == VALID_MSP_VERSION_KEYS.end()) {
        throw UnsupportedAttribute(in["msp_version"].get<string>() + " is not an MSP_VERSION");
    }
    payload->set_msp_version(VALID_MSP_VERSION_KEYS.at(in["msp_version"]));


    validate("capability", in);
    payload->set_capability(in["capability"]);

    return event;
}