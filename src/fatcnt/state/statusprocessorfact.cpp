#include "statusprocessorfact.hpp"

using namespace rrobot;
using namespace std;

StatusProcessorIface* StatusProcessorFactory::createStatusProcessor(Environment env, StateIface* state) {
    StatusProcessor* statusProcessor = new StatusProcessor(state);
    return statusProcessor;
}