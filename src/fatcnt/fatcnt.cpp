#include "fatcnt.hpp"

/**
 * MIT License
 *
 * Copyright (c) 2024 Ryder Robots
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

using namespace rrobot;
namespace po = boost::program_options;

dlib::logger dlog_main("rr_robot_main");

StateIface* volatile state = nullptr;
thread* volatile tmain = nullptr;

/**
 * @fn signalHandler
 * @brief
 * handles system generated interupts.
 * @param signum signale getting sent from system.
 */
void signalHandler(int signum) {
    if (signum == SIGTERM) {
        dlog_main << dlib::LINFO << "SIGTERM has been, begining shutdown process";
        if (state != nullptr) {
            dlog_main << dlib::LINFO << "Setting isRunning to false for handlers";
            state->setIsRunning(false);
        }
        if (tmain != nullptr) {
            while (!tmain->joinable()) {
                dlog_main << dlib::LINFO << "waiting for main loop to start";
                this_thread::sleep_for(chrono::milliseconds(10));
            }
            if (tmain != nullptr && tmain->joinable()) {
                tmain->join();
                dlog_main << dlib::LINFO << "exiting program";
            }
        }
    }
    
    exit(EXIT_SUCCESS);
}

/**
 * @fn main
 * @brief
 * entry point, launches catagorizer (main handler).  Catagorizer launches all other handlers, with configuration 
 * provided from mapper.  Mapper will vary depending on which manifest is given as a command line object, and 
 * provides constants (environment), including specifics on drone hardware.
 * 
 * Mapper will also initilize any handlers needed that are specific to the drone hardware.
 */
int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("manifest,m", po::value<string>(), "manifest file to use")
        ("help,h", "fatcnt creates the interface to the drone and hardware.")
        ("version,v", "prints current version");
    po::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        return EXIT_FAILURE;
    } else if (vm.count("version")) {
        RrVersion version;
        cout << "application:" << version.getAppName() << " version: " + version.getVersion() << "\n";
        return EXIT_FAILURE;
    }

    try {
        string filepath = vm["manifest"].as<string>();
        boost::trim(filepath);
        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        Environment environment = EnviromentProcessor::createEnvironment(manifest);
        dlog_main.set_level(environment.getLogging().getLogLevel());
        RrCatagorizerMapper* mapper = MapperFactory::getMapper(environment);

        dlog_main << dlib::LINFO << "creating state";
        state = StateFactory::createState(environment, mapper->queueNames());
        mapper->init(&environment, state);

        dlog_main << dlib::LINFO << "creating catagorizer";
        RrCatagorizer* catagorizer = new RrCatagorizer();
        catagorizer->init(state, &environment, mapper);

        dlog_main << dlib::LINFO << "starting application";
        std::thread t = std::thread(RrCatagorizer::handleEvent, catagorizer, state);

        tmain = &t;
        while (!t.joinable()) {
            dlog_main << dlib::LINFO << "waiting for main loop to start";
            this_thread::sleep_for(chrono::milliseconds(environment.getQueues().getThreadProcessTime()));
        }
        std::signal(SIGTERM, signalHandler);
        t.join();
        dlog_main << dlib::LINFO << "exitng program";
    } catch (const std::exception &ex) {
        dlog_main << dlib::LFATAL << "fatal error " << ex.what();
    }

    return EXIT_SUCCESS;
}