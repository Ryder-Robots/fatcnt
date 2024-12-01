#include "fatcnt.hpp"

using namespace rrobot;
namespace po = boost::program_options;

dlib::logger dlog_main("rr_robot_main");

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("manifest,m", po::value<string>(), "manifest file to use")
        ("help,h", "fatcnt creates the interface to the drone and hardware.");
        ("version,v", "prints current version")
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
        dlog_main.set_level(dlib::LALL);
        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();

        dlog_main << dlib::LINFO << "creating environment";
        Environment environment = EnviromentProcessor::createEnvironment(manifest);
    } catch (const std::exception &ex) {
        dlog_main << dlib::LFATAL << "fatal error " << ex.what();
    }

    return EXIT_SUCCESS;
}