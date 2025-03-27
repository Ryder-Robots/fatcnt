#include <fatcnt/protocols/common/curators/rmultiwii/rmultiwii_curator.hpp>

using namespace rrobot;

Event* AbstractRmultiiCurator::deserialize(RmMultiWii& in) {
    Event* event;
    if (in.getSize() > 0) {
        event = deserializePayload(in);
    } else {
        event = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    }
    return event;
}

uint8_t* AbstractRmultiiCurator::serialize(Event* in) {
    uint8_t* data;
    if (in->hasPayload()) {
        data = serializePayload(in);
    } else {
        RmMultiWii multiWii = RmMultiWii(in->getCommand(), 0);
        data = multiWii.encode(_crc);
    }
    return data;
}