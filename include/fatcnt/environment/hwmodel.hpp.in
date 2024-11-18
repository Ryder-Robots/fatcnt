#ifndef HWMODEL_HPP
#define HWMODEL_HPP

#include <stdint.h>
#include <fatcnt/protocols/common/mspversion.hpp>
#include <fatcnt/protocols/common/multitype.hpp>

namespace rrobot {
    /**
     * @class HwModel
     * @brief
     * Provides a description of the drone. This can be used for MSP_IDENT responses.
     * (see http://www.multiwii.com/wiki/index.php?title=Multiwii_Serial_Protocol)
     */

    class HwModel {
        public:
            HwModel(MULTITYPE_T multitype, MSP_VERSION mspversion, uint32_t capability) : 
                _multitype(multitype),
                _mspversion(mspversion),
                _capability(capability) {}

            MULTITYPE_T getMultiType() {return _multitype;}
            MSP_VERSION getMspVersion() {return _mspversion;}
            uint32_t    getCapability() {return _capability;}
        private:
            const MULTITYPE_T _multitype;
            const MSP_VERSION _mspversion;
            const uint32_t    _capability;
    };
}

#endif // HWMODEL_HPP