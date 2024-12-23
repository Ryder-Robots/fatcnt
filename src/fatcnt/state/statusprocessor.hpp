#ifndef STATUSPROCESOR_HPP
#define STATUSPROCESOR_HPP

#include <fatcnt/state/statusprocessoriface.hpp>
#include <fatcnt/events/eventhandler.hpp>

using namespace std;

namespace rrobot {
    class StatusProcessor : StatusProcrssorIface {
        public:
            RR_CMMODES getMode(int32_t flags) override;
            
            RRP_STATUS getStatus(int32_t flags) override;

            void addHandler(EventHandler* handler) override {
                _eventHandlers.push_back(handler);
            }

            vector<EventHandler*> getHandlers() {
                return _eventHandlers;
            }

        private:
        vector<EventHandler*> _eventHandlers = {};
    };
}

#endif // STATUSPROCESOR_HPP