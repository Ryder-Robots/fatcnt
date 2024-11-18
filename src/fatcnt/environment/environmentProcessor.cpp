#include "environmentProcessor.hpp"

using namespace rrobot;

dlib::logger dlog_envf("rr_environment_lg");

Environment EnviromentProcessor::createEnvironment(json manifest) {
    dlog_envf.set_level(dlib::LALL);

    HwModel hwModel = createHwModel(manifest);
    RrSerial mc = createMc(manifest);
    RrVersion version;

    return Environment(hwModel, mc, version);
}

HwModel EnviromentProcessor::createHwModel(json manifest) {
    if (!manifest.contains("hwmodel")) {
        throw MissingRequiredAttributeException("missing required attribute for hwmodel");
    }
    string keys[] = {"multitype", "mspversion", "capability"};
    for (auto key : keys) {
        if (!manifest["hwmodel"].contains(key)) {
            throw MissingRequiredAttributeException("missing required attribute " + key + " for hwmodel");
        }
    }

    VALID_MULTITYPE_KEYS_INIT;
    VALID_MSP_VERSION_KEYS_INIT;

    MULTITYPE_T multiType = VALID_MULTITYPE_KEYS.at(manifest["hwmodel"]["multitype"]);
    MSP_VERSION mspVersion = VALID_MSP_VERSION_KEYS.at(manifest["hwmodel"]["mspversion"]);
    uint32_t capability = static_cast<uint32_t>(manifest["hwmodel"]["capability"]);

    return HwModel(multiType, mspVersion, capability);
}

RrSerial EnviromentProcessor::createMc(json manifest) {
    if (!manifest.contains("mc")) {
        throw MissingRequiredAttributeException("missing required attribute for mc");
    }

    string keys[] = {"port", "baud", "charsize", "flow_control", "stop_bits", "parity"};
    for (auto key : keys) {
        if (!manifest["mc"].contains(key)) {
            throw MissingRequiredAttributeException("missing required attribute " + key + " for mc");
        }
    }

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