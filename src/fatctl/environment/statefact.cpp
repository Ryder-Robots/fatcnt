#include "fatctl/environment/statefact.hpp"

using namespace rrobot;
namespace fs = std::filesystem;

dlib::logger dlog_statef("rr_robot_state_factory");
rr_state_c* statefact::create_state(std::string path) {
    try {
        const fs::path filepath =  path; 
        if (!exists(filepath)) {
            dlog_statef << dlib::LFATAL << path << ": is an invalid file";
            throw std::runtime_error("invalid file");           
        }

        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        rr_state_c *state = new rr_state_c();
        state->init();
        state->set_manifest(manifest);

        // detect the handler that needs to be used and place it here.
        rr_handler *handler = new skuld001_handler();
        handler->init();
        state->set_handler(handler);

        return state;
    } catch (const std::exception &ex) {
        dlog_statef << dlib::LFATAL << "attempting to read " << path << ": fatal error occured: " << ex.what();
        throw ex;
    }
}