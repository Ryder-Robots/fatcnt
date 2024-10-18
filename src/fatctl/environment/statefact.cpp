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

        // setup the threads
        skuld001_handler state_hdl = skuld001_handler();
        state_hdl.init();

        pthread_t pid = state->get_event_handler_thread();
        pthread_create(&pid, NULL, &rrobot::skuld001_handler::handle_operations, state);

        return state;
    } catch (const std::exception &ex) {
        dlog_statef << dlib::LFATAL << "attempting to read " << path << ": fatal error occured: " << ex.what();
        throw ex;
    }
}