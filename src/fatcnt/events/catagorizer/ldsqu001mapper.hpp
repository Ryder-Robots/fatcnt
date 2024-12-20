#ifndef LDSQU001MAPPER_HPP
#define LDSQU001MAPPER_HPP

#include <fatcnt/exceptions/exceptions.hpp>
#include <fatcnt/protocols/common/curators/rrp/mspevent.hpp>
#include <fatcnt/events/ui/jseralizer.hpp>
#include <fatcnt/events/uihandler.hpp>
#include <fatcnt/events/statushandler.hpp>
#include "catagorizermapperbase.hpp"

namespace rrobot {

    /**
     * @class LdSqu001Mapper
     * @brief
     * mappings for Landdrone 001
     */
    class LdSqu001Mapper : public CatagorizerMapperBase {
        public:

            vector<EventHandler*> createEventHandlers() override;

            RRP_QUEUES mapQueue(Event* event) override;

            vector<RRP_QUEUES> queueNames() override;

    };
}
#endif // LDSQU001MAPPER_HPP