/**
 * The Scheduler is responsible for running a schedule of FlightModules.
 */
#include <flightmodule/FlightModule.h>

class Scheduler : public FlightModule {
public:
    Scheduler(FlightModule *schedule, int size);
private:
    FlightModule *schedule;
    int size = 0;
};