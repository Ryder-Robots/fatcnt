#include <fatcnt/events/catagorizer.hpp>

using namespace rrobot;

dlib::logger dlog_c("rr_curator");

void RrCatagorizer::init(StateIface* state, Environment* environment, RrCatagorizerMapper* mapper, 
StateManagerIface*  statusProcessor) {
    dlog_c.set_level(environment->getLogging().getLogLevel());
    _state = state;
    _mapper = mapper;
    _environment = environment;
    _statusProcessor = statusProcessor;
    EventHandler::init(state->getQueues(), RRP_QUEUES::CATEGORIZER, RRP_QUEUES::USER_INTERFACE, environment);
}

bool RrCatagorizer::consume(Event* event, StateIface* state) {
    _statusProcessor->push_queue(_mapper->mapQueue(event), event);
    return true;
}

Event* RrCatagorizer::produce(StateIface* state) { return nullptr; }

void RrCatagorizer::setUp() {
    _status = RRP_STATUS::INITILIZING;
    dlog_c << dlib::LINFO << "creating event handlers and initilizing queues";
    vector<EventHandler*> handlers = _mapper->createEventHandlers();

    dlog_c << dlib::LINFO << "creating threads";
    for (EventHandler* handler : handlers) {
        dlog_c << dlib::LINFO << "initilizing handler:" << handler->name();
        std::thread* t = new std::thread(EventHandler::handleEvent, handler, _state);
        while (!t->joinable()) {
            std::this_thread::sleep_for(_state->getQueues()->QUEUE_PROCESS_TIME);
        }
        _threads.push_back(t);
        t->detach();

        if (handler->status() == RRP_STATUS::TERMINATED || handler->status() == RRP_STATUS::ERROR) {
            dlog_c << dlib::LFATAL << "critical error has occurred while starting handler";
            throw FailedToInitilizeHandler("could not initilize handler");
        }

        while (handler->status() != RRP_STATUS::ACTIVE) {
            std::this_thread::sleep_for(_state->getQueues()->QUEUE_PROCESS_TIME);
        }
        dlog_c << dlib::LINFO << "status active for:" << handler->name();
    }

    dlog_c << dlib::LINFO << "Drone is armed - starting catagorizer thread";
    _status = RRP_STATUS::ACTIVE;
}

void RrCatagorizer::tearDown() {
    int threadTimeOut = 0;
    for (EventHandler* handler : _statusProcessor->getHandlers()) {
        dlog_c << dlib::LINFO << "waiting for thread to terminate";
        while (handler->status() != RRP_STATUS::TERMINATED) {
            std::this_thread::sleep_for(_state->getQueues()->QUEUE_PROCESS_TIME);
            threadTimeOut += _environment->getQueues().getThreadProcessTime();

            if (threadTimeOut >= _environment->getQueues().getThreadTimeOut()) {
                dlog_c << dlib::LWARN << "thread has exceeded timeout, forcing shutdown";
                break;
            }
        }
        dlog_c << dlib::LINFO << "removing handler from thread manager";
        delete (handler);
    }
}

bool RrCatagorizer::produceInt(Event* event) {
    const std::lock_guard<std::mutex> lock(*_outbound_lock);
    _outbound_queue->push(event);
    return true;
}
