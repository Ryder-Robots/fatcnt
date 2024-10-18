#ifndef STATEFACT_HPP
#define STATEFACT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <dlib/logger.h>
#include <dlib/misc_api.h>
#include <nlohmann/json.hpp>

#include "fatctl/environment/state.hpp"
#include "fatctl/environment/handlers/handler.hpp"
#include "fatctl/environment/handlers/skuld001_handler.hpp"

using json = nlohmann::json;

/**
 * @brief given manifest config state.
 */
namespace rrobot {
    class statefact {
    public:
        static rr_state_c* create_state(std::string manifest);
    };
}

#endif