/**
 * The Scheduler is responsible for running a schedule of FlightModules.
 */
#include <vector>
#include <flightmodule/FlightModule.h>

class Scheduler : public FlightModule {
public:
    Scheduler(std::vector<FlightModule> schedule);
private:
    std::vector<FlightModule> schedule;
};