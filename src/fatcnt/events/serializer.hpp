#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fatcnt/events/Event.hpp>

namespace rrobot {
    template <typename T> class Serializer {
        public:
        virtual T serialize(Event* event) {return nullptr;};
        virtual Event* deserialize(T event) {return nullptr;};
    };
}

#endif // SERIALIZER_HPP