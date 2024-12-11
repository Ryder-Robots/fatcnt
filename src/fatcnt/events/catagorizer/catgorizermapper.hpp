#ifndef CATAGORIZRMAPPER_HPP
#define CATAGORIZRMAPPER_HPP

#include <vector>
#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/state.hpp>
#include <fatcnt/state/rrpqueues.hpp>
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
            virtual vector<EventHandler*> createEventHandlers(Environment* environment, StateIface* state) {
               vector<EventHandler*> handlers;
               return handlers;
            }
            
            /**
             * @fn mapDirection
             * @brief
             * Applies any any overridden mapping that is a result of changing the drones mode.
             */
            virtual RRP_QUEUES mapQueue(Environment* environment, StateIface* state, Event* event) {
                return RRP_QUEUES::NONE;
            }

            /**
             * @fn initializeMode
             * @brief
             * sets mapper to use a different mode, may also perform any changes to handlers that required.
             * 
             * if the request mode can be assigned for some reason then false is returned otherwise true.
             */
            virtual bool initializeMode(Environment* environment, StateIface* state, RR_CMMODES mode) {
                return false;    
            }

            /**
             * @fn getQueues
             * @brief
             * returns a list of supported queue names.
             */
            virtual vector<RRP_QUEUES> getQueues() {
                vector<RRP_QUEUES> directions;
                return directions;
            }
    };
}

#endif // CATAGORIZRMAPPER_HPP