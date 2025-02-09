#include "mapperfactory.hpp"

using namespace rrobot;

dlib::logger dlog_mapper_fact("rr_mapper_fact");


RrCatagorizerMapper* MapperFactory::getMapper(rrobot::Environment env) {
    RrCatagorizerMapper* mapper = nullptr;
    dlog_mapper_fact.set_level(env.getLogging().getLogLevel());

    switch(env.getHwModel().getMspVersion()) {
        case MSP_VERSION::VIRTUAL:
        case MSP_VERSION::SKULD001:
        case MSP_VERSION::SKULD002:
            mapper = reinterpret_cast<RrCatagorizerMapper*>(new LdSqu001Mapper());
    }

    if (mapper == nullptr) {
        dlog_mapper_fact << dlib::LERROR << "MSP_VERSION is not supported by this version of " + env.getVersion().getAppName() 
            + " version:" + env.getVersion().getVersion();
        throw UnsupportedAttribute("MSP_VERSION is not supported by this version of " + env.getVersion().getAppName() 
            + " version:" + env.getVersion().getVersion());
    }
    return mapper;
}