#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <fatcnt/environment/hwmodel.hpp>
#include <fatcnt/environment/rrserial.hpp>
#include <fatcnt/environment/version.hpp>

namespace rrobot {

    /**
     * @class Environment
     * @brief
     * Returns constants that are set either through environment variables, program manifest, or via command line options.
     * These values should not change during the duration of the program.
     */
    class Environment {
        public:
            Environment(const HwModel hwmodel, const RrSerial rrSerial, const RrVersion rrVersion) :
                _hwModel(hwmodel),
                _rrSerial(rrSerial),
                _version(rrVersion) {}

            const HwModel getHwModel() {return _hwModel;}
            const RrSerial getMc() {return _mc;}
            const RrVersion getVersion() { return _version;}
            
        private:
            const HwModel   _hwModel;
            const RrSerial  _mc;
            const RrVersion _version;
    };
}

#endif // ENVIRONMENT_HPP