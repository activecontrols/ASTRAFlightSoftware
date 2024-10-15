#include <flightmodule/Scheduler.h>

/**
 * Construct a scheduler with a list of modules.
 * Must pass correct size.
 */
Scheduler::Scheduler(std::vector<FlightModule> schedule) {
    this->schedule = schedule;
}

/**
 * Call inits for each module.
 */
Scheduler::init() {
    for (int i = 0; i < this->schedule.size(); ++i) {
        this->schedule[i].init();
    }
}

/**
 * Call updates for each module.
 */
Scheduler::update(unsigned long time) {
    for (int i = 0; i < this->schedule.size(); ++i) {
        this->schedule[i].update();
    }
}