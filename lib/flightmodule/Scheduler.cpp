#include "Scheduler.h"
#include <iostream>

/**
 * Construct a scheduler with a list of modules.
 * Must pass correct size.
 */
Scheduler::Scheduler(std::vector<FlightModule*> &schedule) {
    this->schedule = schedule;
}

/**
 * Call inits for each module.
 */
int Scheduler::init() {
    for (unsigned int i = 0; i < this->schedule.size(); ++i) {
        this->schedule[i]->init();
    }
    return NO_ERROR_CODE;
}

/**
 * Call updates for each module.
 */
void Scheduler::update(unsigned long time) {
    for (unsigned int i = 0; i < this->schedule.size(); ++i) {
        this->schedule[i]->update(time);
    }
}