#ifndef CATAGORIZRMAPPER_HPP
#define CATAGORIZRMAPPER_HPP

#include <vector>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/protocols/common/mspdirection.hpp>
#include <fatcnt/events/eventhandler.hpp>
#include <fatcnt/events/Event.hpp>
#include <fatcnt/events/catagorizer/cmodes.hpp>

using namespace std;

namespace rrobot {

    /**
     * @class RrCatagorizerMapper
     * @brief
     * implemention of a specific drone. Defines the specific feature, and handlers that a drone will use.
     */
    class RrCatagorizerMapper {
        public:
            /**
             * @fn createEventHandlers
             * @brief
             * creates the event handler mapping.
             */
            virtual vector<EventHandler*> createEventHandlers(Environment environment, StateIface state) = 0;
            
            /**
             * @fn mapDirection
             * @brief
             * Applies any any overridden mapping that is a result of changing the drones mode.
             */
            virtual MSPDIRECTION mapDirection(Event event, RR_CMMODES mode) = 0;

            /**
             * @fn getDirections
             * @brief
             * returns a list of supported directions.
             */
            virtual vector<MSPDIRECTION> getDirections() = 0;
    };
}

#endif // CATAGORIZRMAPPER_HPP