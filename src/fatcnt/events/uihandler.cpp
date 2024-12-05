#include "uihandler.hpp"

using namespace rrobot;
using namespace std;
using json = nlohmann::json;

dlib::logger dlog_ui("rr_robot_ui");

void UiHandler::init(External *external, StateIface* state, Serializer<json>* serializer) {
    /**
     * forwards messages from USER_INTERFACE to catagorizer, if they are authenticated
     * messages from catagorizer are sent to socket.
     */
    dlog_ui.set_level(dlib::LALL);
    dlog_ui << dlib::LINFO << "configurating queues";

    EventHandler::init(state->getQueues(), MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);
    _external = external;
    _buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char)));
    _serializer = serializer;
}

bool UiHandler::consume(Event* event, StateIface* state) {
    _available = false;
    json out = _serializer->serialize(event);
    string output = out.dump();
    _external->send_rr(output.c_str(),  output.length() * sizeof(char));
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
    if (!(j.contains("command") && j.contains("payload"))) {
        dlog_ui << dlib::LERROR << "command and payload are required acttributes";
        throw MissingRequiredAttributeException("command and payload are required acttributes");
    }

    Event* event = _serializer->deserialize(j);
    _available = true;
    return event;
}

bool UiHandler::available() {
    return _external->available() && _available;
}