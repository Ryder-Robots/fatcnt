#ifndef CATAGORIZRMAPPER_HPP
#define CATAGORIZRMAPPER_HPP

#include <vector>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/state/rrpqueues.hpp>
#include <fatcnt/state/statemanageriface.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/events/catagorizer/cmodes.hpp>
#include <fatcnt/com/server.hpp>

using namespace std;

namespace rrobot {

    /**
     * @class RrCatagorizerMapper
     * @brief
     * implemention of a specific drone. Defines the specific feature, and handlers that a drone will use.
     */
    class RrCatagorizerMapper {
        public:

            virtual void init(Environment* environment, StateIface* state, StateManagerIface* statusProcessor) = 0;

            /**
             * @fn createEventHandlers
             * @brief
             * creates the event handler mapping.
             */
            virtual vector<EventHandler*> createEventHandlers() = 0;
            
            /**
             * @fn mapDirection
             * @brief
             * Applies any any overridden mapping that is a result of changing the drones mode.
             */
            virtual RRP_QUEUES mapQueue(Event* event) = 0;

            /**
             * @fn queueNames
             * @brief
             * Returns a vector of queue names, this is used to create queues for drone.
             */
            virtual vector<RRP_QUEUES> queueNames() = 0;
    };
}

#endif // CATAGORIZRMAPPER_HPP