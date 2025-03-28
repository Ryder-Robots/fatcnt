#include <fatcnt/protocols/common/curators/rmultiwii/rmultiwii_none_curator.hpp>
#include <fatcnt/protocols/common/curators/rrp/msp_none.hpp>

using namespace rrobot;

/*
 * None does not have a payload.
 */
uint8_t* RrMultiWiiCuratorNone::serializePayload(Event* in) {
    uint8_t* data;
    return data;
}