#ifndef ROUTER_H
#define ROUTER_H
#include "Scheduler.h"
#include <map>
#include <unordered_map>
#include <functional>

enum State {
    ASTRA_PREBOOT,
    ASTRA_MAINLOOP,
};

class Router : public FlightModule {
public:
    Router();
    int init() override;
    void update(unsigned long time) override;

    int call(int commandID, float params[7]); 
    void registerRoute(int commandID, std::function<int(float[7])> callback); // changed to std::function for easier insertion of routes
    void registerSchedule(enum State state, Scheduler *modules);
    int changeSchedule(enum State state);
private:
    std::multimap<int, std::function<int(float[7])>> callbacks; // why use multimap?? are we threaded? we should be single
    std::unordered_map<int, Scheduler*> schedules;
    Scheduler *currentSchedule = nullptr;
    Scheduler *nextSchedule = nullptr;
};

#endif //ROUTER_H