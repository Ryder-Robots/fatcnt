#include "environmentProcessor.hpp"

using namespace rrobot;

Environment EnviromentProcessor::createEnvironment(json manifest) {

    HwModel hwModel = createHwModel(manifest);
    RrSerial mc = createMc(manifest);
    RrVersion version;
    Queues queues = createQueues(manifest);
    Server server = createServer(manifest);

    return Environment(hwModel, mc, version, queues, server);
}

Environment* EnviromentProcessor::createEnvironmentRef(json manifest) {

    HwModel hwModel = createHwModel(manifest);
    RrSerial mc = createMc(manifest);
    RrVersion version;
    Queues queues = createQueues(manifest);
    Server server = createServer(manifest);

    return new Environment(hwModel, mc, version, queues, server);
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
    vector<string> keys = {"limit", "thread_wait_time", "thread_process_time", "thread_time_out"};
    verify(manifest, keys, "queues");
    return Queues(manifest["queues"]["limit"], 
        manifest["queues"]["thread_wait_time"],  
        manifest["queues"]["thread_process_time"],
        manifest["queues"]["thread_time_out"]);
}

Server EnviromentProcessor::createServer(json manifest) {
    vector<string> keys = {"port", "maxconnections"};
    verify(manifest, keys, "server");
    return Server(manifest["server"]["port"], manifest["server"]["maxconnections"]);
}