#include <fatcnt/events/ai/ai_virtual_predictor.hpp>

using namespace rrobot;

void AiVirtualPredictor::addFeature(Event* event) {
    _event = event;
}

Event* AiVirtualPredictor::predict() {
    return _convertor->convert(_event);
}