/**
 * @brief main header file for project
 */

#ifndef FATCTL_H
#define FATCTL_H

#include <dlib/logger.h>
#include <dlib/misc_api.h>
#include <boost/program_options.hpp>

#include "fatctl/ui/server.hpp"
#include "fatctl/environment/state.hpp"
#include "fatctl/environment/statefact.hpp"

#define DEFAULT_PORT 8081
#define DEFAULT_MANIFEST "/opt/rrobots/maifest.json"

#endif