#include "environmentProcessor.hpp"

using namespace rrobot;
namespace fs = std::filesystem;

dlib::logger dlog_envf("rr_environment_lg");

Environment EnviromentProcessor::createEnvironment(string manifest, po::variables_map vm) {
    dlog_envf.set_level(dlib::LALL);
    try {
        const fs::path filepath = manifest;
        if (!exists(filepath)) {
            dlog_envf << dlib::LFATAL << manifest << ": is an invalid file";
            throw InvalidManifestException("invalid file " + manifest);
        }

        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        HwModel hwModel = createHwModel(manifest);
    } catch (const std::exception &ex) {
    }
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

    LibSerial::BaudRate baudRate = VALID_BAUDRATES.at(manifest["mc"]["baud"]);
    LibSerial::CharacterSize charsize = VALID_CHAR_SZ.at(manifest["mc"]["charsize"]);
    LibSerial::FlowControl flowControl = VALID_FLOW_CNT.at(manifest["mc"]["flow_control"]);
    LibSerial::StopBits stopBits = VALID_STOPBIT.at(manifest["mc"]["stop_bits"]);
    LibSerial::Parity parity = VALID_PARITY.at(manifest["mc"]["parity"]);

    return RrSerial();
}