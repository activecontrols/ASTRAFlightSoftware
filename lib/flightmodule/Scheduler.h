/**
 * The Scheduler is responsible for running a schedule of FlightModules.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FlightModule.h"

class Scheduler : public FlightModule {
public:
    Scheduler(FlightModule**, int scheduleSize);
    int init() override;
    void update(unsigned long time) override;
    static void taskFunction(void* pvParameters);
    void createTasks();
private:
    FlightModule** schedule;
    unsigned int size;
};

#endif // SCHEDULER_H