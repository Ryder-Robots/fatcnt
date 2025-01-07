#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fatcnt/events/Event.hpp>

namespace rrobot {
    template <typename T, typename E> class Serializer {
        public:

        /**
         * @fn serialize 
         * @brief
         * serialize event.
         */
        virtual T serialize(Event* event) {return nullptr;};

        /**
         * @fn deserialize
         * @brief
         * deserialize inbound event based upon the direction given in event.
         */
        virtual E deserialize(T event) {return nullptr;};

        /**
         * @fn deserialize
         * @brief
         * when direction is defined then override the direction on the inbound variable.
         * 
         */
        virtual E deserialize(T event, MSPDIRECTION direction) {return deserialize(event);}
    };
}

#endif // SERIALIZER_HPP