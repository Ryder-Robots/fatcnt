#include "fatctl/environment/statefact.hpp"

using namespace rrobot;
namespace fs = std::filesystem;

dlib::logger dlog_statef("rr_robot_state_factory");
rr_state_c statefact::create_state(std::string path, std::string manifest) {
    try {
        const fs::path manifestp{path};
        if (!exists(manifestp)) {
             dlog_statef << dlib::LFATAL << path << ": is an invalid path";
             throw std::runtime_error("invalid path");
        }
        fs::path filepath = manifestp / manifest; 
        if (!exists(filepath)) {
            dlog_statef << dlib::LFATAL << manifest << ": is an invalid file";
            throw std::runtime_error("invalid file");           
        }

        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        rr_state_c state = rr_state_c();
        state.set_manifest(manifest);
        return state;
    } catch (const std::exception &ex) {
        dlog_statef << dlib::LFATAL << "attempting to read " << manifest << ": fatal error occured: " << ex.what();
        throw ex;
    }
}