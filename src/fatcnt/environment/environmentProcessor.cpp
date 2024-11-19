#include "environmentProcessor.hpp"

using namespace rrobot;

dlib::logger dlog_envf("rr_environment_lg");

Environment EnviromentProcessor::createEnvironment(json manifest) {
    dlog_envf.set_level(dlib::LALL);

    HwModel hwModel = createHwModel(manifest);
    RrSerial mc = createMc(manifest);
    RrVersion version;
    Queues queues = createQueues(manifest);

    return Environment(hwModel, mc, version, queues);
}

HwModel EnviromentProcessor::createHwModel(json manifest) {
    vector<string> keys = {"multitype", "mspversion", "capability"};
    verify(manifest, keys, "hwmodel");

    VALID_MULTITYPE_KEYS_INIT;
    VALID_MSP_VERSION_KEYS_INIT;

    MULTITYPE_T multiType = VALID_MULTITYPE_KEYS.at(manifest["hwmodel"]["multitype"]);
    MSP_VERSION mspVersion = VALID_MSP_VERSION_KEYS.at(manifest["hwmodel"]["mspversion"]);
    uint32_t capability = static_cast<uint32_t>(manifest["hwmodel"]["capability"]);

    return HwModel(multiType, mspVersion, capability);
}

RrSerial EnviromentProcessor::createMc(json manifest) {
    vector<string> keys = {"port", "baud", "charsize", "flow_control", "stop_bits", "parity"};
    verify(manifest, keys, "mc");

    VALID_BAUDRATES_INIT;
    VALID_CHAR_SZ_INIT;
    VALID_FLOW_CNT_INIT;
    VALID_STOPBIT_INIT;
    VALID_PARITY_INIT;

    LibSerial::BaudRate baudRate = VALID_BAUDRATES.at(manifest["mc"]["baud"]);
    LibSerial::CharacterSize charsize = VALID_CHAR_SZ.at(manifest["mc"]["charsize"]);
    LibSerial::FlowControl flowControl = VALID_FLOW_CNT.at(manifest["mc"]["flow_control"]);
    LibSerial::StopBits stopBits = VALID_STOPBIT.at(manifest["mc"]["stop_bits"]);
    LibSerial::Parity parity = VALID_PARITY.at(manifest["mc"]["parity"]);

    return RrSerial(manifest["mc"]["port"], baudRate, charsize, flowControl, stopBits, parity);
}


Queues EnviromentProcessor::createQueues(json manifest) {
    vector<string> keys = {"limit", "thread_wait_time", "thread_process_time"};
    verify(manifest, keys, "queues");
    return Queues(manifest["queues"]["limit"], 
        manifest["queues"]["thread_wait_time"],  
        manifest["queues"]["thread_process_time"]);
}