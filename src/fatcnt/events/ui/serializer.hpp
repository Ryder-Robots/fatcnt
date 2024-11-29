#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fatcnt/events/Event.hpp>

namespace rrobot {
    template <typename T> class Serializer {
        public:
        virtual Event* serialize(T event);
    };
}

#endif // SERIALIZER_HPP