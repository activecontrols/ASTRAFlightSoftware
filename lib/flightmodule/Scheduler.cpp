#include <flightmodule/Scheduler.h>

/**
 * Construct a scheduler with a list of modules.
 * Must pass correct size.
 */
Scheduler::Scheduler(FlightModule schedule[], int size) {
    this->schedule = schedule;
    this->size = size;
}

/**
 * Call inits for each module.
 */
Scheduler::init() {
    for (int i = 0; i < size; ++i) {
        this->schedule[i].init();
    }
}

/**
 * Call updates for each module.
 */
Scheduler::update(unsigned long time) {
    for (int i = 0; i < size; ++i) {
        this->schedule[i].update();
    }
}