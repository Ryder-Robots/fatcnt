#include "uihandler.hpp"

using namespace rrobot;
using namespace std;
using json = nlohmann::json;

dlib::logger dlog_ui("rr_robot_ui");

void UiHandler::init(External* external, Environment* environment, StateIface* state, Serializer<json>* serializer) {
    /**
     * forwards messages from USER_INTERFACE to catagorizer, if they are authenticated
     * messages from catagorizer are sent to socket.
     */
    dlog_ui.set_level(dlib::LALL);
    dlog_ui << dlib::LINFO << "configurating queues";

    EventHandler::init(state->getQueues(), RRP_QUEUES::USER_INTERFACE, RRP_QUEUES::CATEGORIZER);

    _external = external;
    _buffer = static_cast<char*>(malloc(BUFSIZ * sizeof(char)));
    _serializer = serializer;
    _environment = environment;
    _state = state;
}

bool UiHandler::consume(Event* event, StateIface* state) {
    _available = false;
    json out = _serializer->serialize(event);
    string output = out.dump() + _delimiter;
    if (_external->send_rr(output.c_str(), output.length() * sizeof(char)) == -1) {
        dlog_ui << dlib::LFATAL
            << "sommething went wrong when accepting connection: " + to_string(errno) + ": " + strerror(errno);
        throw BadConnectionException("sommething went wrong when accepting connection: " + to_string(errno) + ": " +
            strerror(errno));        
    }
    _available = true;
    return true;
}

Event* UiHandler::produce(StateIface* state) {
    _available = false;

    size_t pos = 0;
    memset(_buffer, 0, BUFSIZ);

    while (pos != BUFSIZ) {
        if (pos == BUFSIZ) {
            dlog_ui << dlib::LERROR << "excessively large message has been recieved";
            throw ExceedMaxMessage("excessively large message has been recieved");
        }
        char c;
        size_t n = _external->recv_rr(&c, sizeof(char));

        if (n != 1) {
            dlog_ui << dlib::LERROR << "message sent without deliemeter";
            throw InvalidMessageSent("message sent without deliemeter");
        }

        if (c == _delimiter) {
            break;
        }
        _buffer[pos++] = c;
    }
    string token = _buffer;
    if (!json::accept(token)) {
        dlog_ui << dlib::LERROR << "inbound message is not valid JSON";
        throw InvalidFormat("inbound message is not valid JSON");
    }

    json j = json::parse(token);
    if (!j.contains("command")) {
        dlog_ui << dlib::LERROR << "command is a required attribute";
        throw MissingRequiredAttributeException("command is a required attribute");
    }

    Event* event = _serializer->deserialize(j, MSPDIRECTION::EXTERNAL_IN);
    _available = true;
    return event;
}

bool UiHandler::available() {
    if (_available) {
        ssize_t availableBytes = _external->available();
        if (availableBytes <= 0) {
            _available = false;
        }
    }
    return _available; 
}


void UiHandler::setUp() {
    // start the listener inside the loop, so that other processes are not delayed while waiting for 
    // user interface to connect.
    _external->init(_environment, _state);
    dlog_ui << dlib::LINFO << "changing to active state to listen to port";
    setStatus(RRP_STATUS::ACTIVE);
    if (_external->accept_rr() == -1) {
        dlog_ui << dlib::LFATAL
            << "sommething went wrong when accepting connection: " + to_string(errno) + ": " + strerror(errno);
        throw BadConnectionException("sommething went wrong when accepting connection: " + to_string(errno) + ": " +
            strerror(errno));
    }
    dlog_ui << dlib::LINFO << " established successful connection";
}

void UiHandler::tearDown() {
    dlog_ui << dlib::LINFO << "connecton is closed";
    _external->close_rr();
}

void UiHandler::reload() {
    dlog_ui << dlib::LERROR << "attempting to stop and re-establish connection";
    setUp();
    tearDown();
    setStatus(RRP_STATUS::ACTIVE);
}