/**
 * The Scheduler is responsible for running a schedule of FlightModules.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <flightmodule/FlightModule.h>

class Scheduler : public FlightModule {
public:
    Scheduler(std::vector<FlightModule> schedule);
private:
    std::vector<FlightModule> schedule;
};

#endif // SCHEDULER_H