#include "mapperfactory.hpp"

using namespace rrobot;

RrCatagorizerMapper* MapperFactory::getMapper(rrobot::Environment env) {
    RrCatagorizerMapper* mapper = nullptr;

    switch(env.getHwModel().getMspVersion()) {
        case MSP_VERSION::VIRTUAL:
        case MSP_VERSION::SKULD001:
            mapper = reinterpret_cast<RrCatagorizerMapper*>(new LdSqu001Mapper());
    }

    if (mapper == nullptr) {
        throw UnsupportedAttribute("MSP_VERSION is not supported by this version of " + env.getVersion().getAppName() 
            + " version:" + env.getVersion().getVersion());
    }
    return mapper;
}