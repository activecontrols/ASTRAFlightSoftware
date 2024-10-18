/**
 * The Scheduler is responsible for running a schedule of FlightModules.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "FlightModule.h"

class Scheduler : public FlightModule {
public:
    Scheduler(std::vector<FlightModule> schedule);
    int init() override;
    void update(unsigned long time) override;
private:
    std::vector<FlightModule> schedule;
};

#endif // SCHEDULER_H