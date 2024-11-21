#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <fatcnt/environment/environment.hpp>
#include <fatcnt/state/state.hpp>

namespace rrobot {
    /**
     * Event handlers, handle a specific type of event.
     * 
     * E is the environment for inital configuration
     */
    template <typename E> class EventHandler {
        private:

        /**
         * @fn init
         * @param environment environment class
         * @brief
         * This method is called when program is reloaded, or initialization time by the queue manger
         * 
         */
        virtual void init(E environment) {}

        State *_state;
    };


    template <typename E> class EventHandlerConsumer : public EventHandler<E> {
        private:

        virtual void consume(Event event) {}
    };

    
    template <typename E> class EventHandlerProducer : public EventHandler<E> {
        private:

        virtual Event event produce() {}
    };

    template <typename E> class EventHandlerBoth : public EventHandler<E> {
        private:

        virtual Event event produce() {}
        virtual void consume(Event event) {}
    };
}

#endif // EVENTHANDLER_HPP