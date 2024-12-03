#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fatcnt/events/Event.hpp>

namespace rrobot {
    template <typename T> class Serializer {
        public:
        virtual T serialize(Event* event) = 0;
        virtual Event* deserialize(T event) = 0;
    };
}

#endif // SERIALIZER_HPP