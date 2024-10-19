#include "Router.h"
#include "Scheduler.h"
#include "Error.h"
#include <unordered_map>

Router::Router() {}

int Router::init() {
    return this->currentSchedule->init();
}

void Router::update(unsigned long time) {
    this->currentSchedule->update(time);
    if (this->nextSchedule != nullptr) {
        this->currentSchedule = this->nextSchedule;
        this->currentSchedule->init();
        this->nextSchedule = nullptr;
    }
}

int Router::call(int commandID, float params[7]) {
    if (this->callbacks.find(commandID) == this->callbacks.end()) {
        return -1; // No command registered
    }

    auto range = this->callbacks.equal_range(commandID);
    int status = 0;
    for (auto i = range.first; i != range.second; ++i) {
        int ret = i->second(params);
        if (ret != 0) status = ret;
    }
    return status;
}

void Router::registerRoute(int command_id, int (*callback)(float params[7])) {
    this->callbacks.insert({command_id, callback});
}

void Router::registerSchedule(enum State state, Scheduler *modules) {
    this->schedules[state] = modules;
}

int Router::changeSchedule(enum State state) {
    this->nextSchedule = this->schedules[state];
    return NO_ERROR_CODE;
}