#include "Scheduler.h"

/**
 * Construct a scheduler with a list of modules.
 * Must pass correct size.
 */
Scheduler::Scheduler(FlightModule** schedule, int scheduleSize) {
    this->schedule = schedule;
    this->size = scheduleSize;
}

/**
 * Call inits for each module.
 */
int Scheduler::init()
{
    for (unsigned int i = 0; i < size; ++i) {
        this->schedule[i]->init();
    }
    return NO_ERROR_CODE;
}

/**
 * Call updates for each module.
 */
void Scheduler::update(unsigned long time) {
    for (unsigned int i = 0; i < size; ++i) {
        this->schedule[i]->update(time);
    }
}