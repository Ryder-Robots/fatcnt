#include <fatcnt/main.hpp>

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
void FatController::signalHandler(int signum) {
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
int FatController::run(int argc, char *argv[]) {
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

        Environment environment =  _mapper->getEnvironmentalProcessor()->createEnvironment(manifest);
        dlog_main.set_level(environment.getLogging().getLogLevel());

        dlog_main << dlib::LINFO << "creating state";
        state = StateFactory::createState(environment, _mapper->queueNames());
        StateManagerIface* stateManager = new StateManager(state, &environment);
        _mapper->init(&environment, state, stateManager);

        dlog_main << dlib::LINFO << "creating catagorizer";
        RrCatagorizer* catagorizer = new RrCatagorizer();
        catagorizer->init(state, &environment, _mapper, stateManager);

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