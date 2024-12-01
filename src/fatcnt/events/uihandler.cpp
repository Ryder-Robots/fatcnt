#include "uihandler.hpp"

using namespace rrobot;
using namespace std;
using json = nlohmann::json;

dlib::logger dlog_ui("rr_robot_ui");

void UiHandler::init(Authenticator *authenticator, External *external, RrQueues* queues, StateIface* state) {
    try {
        /**
         * forwards messages from USER_INTERFACE to catagorizer, if they are authenticated
         * messages from catagorizer are sent to socket.
         */
        dlog_ui.set_level(dlib::LALL);

        dlog_ui << dlib::LINFO << "configurating queues";
        EventHandler::init(queues, MSPDIRECTION::USER_INTERFACE, MSPDIRECTION::CATEGORIZER);

        _authenticator = authenticator;
        _external = external;
        _buffer = static_cast<char *>(malloc(BUFSIZ * sizeof(char)));

        dlog_ui << dlib::LINFO << "waiting to authenicate";
        while(!state->isAuthenticated() && state->isRunning()) {
            if (_external->available() && _authenticator->authenticate(recieve())) {
                state->setIsAuthenticated(true);
            }
        }

    } catch (const exception& e) {
        dlog_ui << dlib::LFATAL << "critical error occurred: " << e.what();
        state->setIsRunning(false);
    } catch(...) {
        dlog_ui << dlib::LFATAL << "critical error occurred: UNKNOWN";
        state->setIsRunning(false);
    }
}

Event* UiHandler::recieve() {
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

    // TODO deserialize.
    // search for required parts
    if (!(j.contains("action") && j.contains("payload"))) {
        dlog_ui << dlib::LERROR << "action and payload are required acttributes";
        throw MissingRequiredAttributeException("action and payload are required acttributes");
    }
}

bool UiHandler::consume(Event* event, StateIface* state) {
}

Event* UiHandler::produce(StateIface* state) {
    return recieve();
}

bool UiHandler::available() {return true;}